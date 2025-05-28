// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/States/AttackState.h"
#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "StateTreeExecutionContext.h"
#include "Gameplay/Components/AC_AbilitySet.h"


UAC_StateManager::UAC_StateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_StateManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemyBase = Cast<AGAS_EnemyBase>(GetOwner());
	if (!OwnerEnemyBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyBase is null in: %s !"), *GetName());
		return;
	}

	OwnerEnemyBase->GetAbilitySetComponent()->OnAbilitySetGiven.AddDynamic(this, &UAC_StateManager::OnAbilitySetGiven);

	OwnerController = Cast<AAIControllerBase>(OwnerEnemyBase->GetController());
	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
		return;
	}

	BehaviorDecisionComponent = OwnerController->GetBehaviorDecisionComponent();
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	OwnerStateTree = OwnerController->GetStateTreeComponent();
	if (!OwnerStateTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerStateTree is null in: %s !"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemyBase->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyASC is null in: %s !"), *GetName());
		return;
	}
} 

void UAC_StateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UStateBase* State : StateInstances)
	{
		State->OnExit();
	}

	Super::EndPlay(EndPlayReason);
}

void UAC_StateManager::OnAbilitySetGiven(const AActor* OwnerActor)
{
	CreateStates();
	StartLogic();
}

void UAC_StateManager::CreateStates()
{
	FStateInitParams StateInitParams = FStateInitParams(OwnerEnemyBase, OwnerController, OwnerEnemyASC, OwnerController->GetTargetActor(), BehaviorDecisionComponent, this);

	for (TSubclassOf<UStateBase> StateClass : StateClassArray)
	{
		if (!*StateClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid state class in array."));
			continue;
		}

		UStateBase* NewState = NewObject<UStateBase>(this, StateClass);
		if (!NewState || !NewState->StateTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate: %s"), *StateClass->GetName());
			continue;
		}

		NewState->StateInitalize(StateInitParams);
		StateInstances.Add(NewState);
	}
}

void UAC_StateManager::StartLogic()
{
	RequestStateTreeEnter(StartState);
}

void UAC_StateManager::StopLogic()
{
	bActive = false;
}

void UAC_StateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->OnTick(DeltaTime);

		if (bEnableDebug) 
		{
			const FVector Location = OwnerEnemyBase->GetActorLocation() + FVector(0.f, 0.f, 150.f);
			const FString DebugText = FString::Printf(TEXT("State: %s"), *CurrentState->GetName());
			DrawDebugString(GetWorld(), Location, DebugText, nullptr, FColor::Cyan, 0.f, true, 1.5f);
		}
	}
}

void UAC_StateManager::RequestStateTreeEnter(const FGameplayTag& StateTag)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return;
	}

	// Find the instance of the requested state
	for (UStateBase* State : StateInstances)
	{
		if (State && State->StateTag == StateTag)
		{
			if (State->EnterCondition()) 
			{
				if (CurrentState)
				{
					CurrentState->OnExit();
				}

				State->OnEnter();
				CurrentState = State;
				return;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Condition of %s is false, cannot enter"), *State->GetName());
			}
		}
	}
}

void UAC_StateManager::RequestStateTreeExit(const FGameplayTag& StateTag, const FGameplayTag& TransactionTag)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return;
	}

	// If request coming with trancastion tag we directly enter
	if (TransactionTag.IsValid()) 
	{
		RequestStateTreeEnter(TransactionTag);
		return;
	}

	if (StateTag == GAS_Tags::TAG_AI_State_Movement)
	{
		ExitFromMovementState();
	}
	else if (StateTag == GAS_Tags::TAG_AI_State_Attack)
	{
		ExitFromAttackState();
	}
	else if (StateTag == GAS_Tags::TAG_AI_State_InComingAttack)
	{
		ExitFromInComingAttackState();
	}
}

void UAC_StateManager::ExitFromMovementState()
{
	
}

void UAC_StateManager::ExitFromAttackState()
{
	/*
	if (bInComingAttack)
	{
		OwnerStateTree->SendStateTreeEvent(TransactionTag);
		return;
	}
	*/

	FAttackData NewSelectedAttack = SelectNewBestAttack();
	if (IsAttackInRange(NewSelectedAttack.AbilityClass))
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Attack);
	}
	else
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Movement);
	}
}

void UAC_StateManager::ExitFromInComingAttackState()
{
	FAttackData NewSelectedAttack = SelectNewBestAttack();
	if (IsAttackInRange(NewSelectedAttack.AbilityClass))
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Attack);
	}
	else
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Movement);
	}
}

float UAC_StateManager::GetTargetDistance() const
{
	if (!OwnerController)
	{
		return -1.0f;
	}

	return OwnerController->GetTargetHeroDistance();
}

bool UAC_StateManager::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	UGAS_GameplayAbilityBase* AbilityCDO = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();

	if (AbilityCDO->MaxRange > GetTargetDistance() && AbilityCDO->MinRange < GetTargetDistance())
	{
		return true;
	}

	return false;
}

FAttackData UAC_StateManager::SelectNewBestAttack()
{
	FAttackData NewAttackData = BehaviorDecisionComponent->GetBestAttack();
	LastSelectedAttackData = NewAttackData;
	return NewAttackData;
}






