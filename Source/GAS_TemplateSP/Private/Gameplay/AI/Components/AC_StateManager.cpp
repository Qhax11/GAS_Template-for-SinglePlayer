// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"


UAC_StateManager::UAC_StateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_StateManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AAIControllerBase>(GetOwner());
	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
		return;
	}

	OwnerEnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
	if (!OwnerEnemyBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyBase is null in: %s !"), *GetName());
		return;
	}
	OwnerEnemyBase->GetAbilitySetComponent()->OnAbilitySetGiven.AddDynamic(this, &UAC_StateManager::OnAbilitySetGiven);

	BehaviorDecisionComponent = OwnerController->GetBehaviorDecisionComponent();
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemyBase->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyASC is null in: %s !"), *GetName());
		return;
	}

	EnemyTagDelegatesComponent = OwnerEnemyBase->GetTagDelegatesComponent();
	if (!EnemyTagDelegatesComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyTagDelegatesComponent is null in: %s !"), *GetName());
		return;
	}
} 

void UAC_StateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentState)
	{
		CurrentState->OnExit();
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
	FStateInitParams StateInitParams = FStateInitParams(OwnerEnemyBase, OwnerController, OwnerEnemyASC, OwnerController->GetTargetActor(), EnemyTagDelegatesComponent, BehaviorDecisionComponent, this);

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

void UAC_StateManager::OnTargetDetected()
{
	if (!CurrentState) 
	{
		return;
	}

	if (CurrentState->StateTag == GAS_Tags::TAG_AI_State_InComingAttack) 
	{
		return;
	}

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

void UAC_StateManager::RequestStateTreeEnter(const FGameplayTag& StateTag)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return;
	}

	if (bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s state has been requested to enter"), *StateTag.ToString());
	}

	UStateBase* FindedState = GetStateWithTag(StateTag);
	if (!FindedState) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s FindedState is null!"));
		return;
	}

	if (FindedState->EnterCondition())
	{
		if (CurrentState)
		{
			CurrentState->OnExit();
		}

		FindedState->OnEnter();
		CurrentState = FindedState;
		return;
	}
	else
	{
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("[State Manager]: Condition of %s is false, cannot enter"), *FindedState->GetName());
		}
	}
}

void UAC_StateManager::RequestStateTreeExit(const FGameplayTag& StateTag, const FGameplayTag& TransactionTag, FString Reason)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return;
	}

	if (bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s state has been requested to exit, reason is: %s"), *StateTag.ToString(), *Reason);
	}

	UStateBase* FindedState = GetStateWithTag(StateTag);
	if (!FindedState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s FindedState is null!"));
		return;
	}

	if (!FindedState->ExitCondition()) 
	{
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("[State Manager]: Condition of %s is false, cannot exit"), *StateTag.ToString());
		}
		return;
	}

	// If request coming with trancastion tag we directly enter
	if (TransactionTag.IsValid()) 
	{
		RequestStateTreeEnter(TransactionTag);
		return;
	}

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
	if (!AbilityClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Class is null in: %s"), *GetName());
		return false;
	}

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

UStateBase* UAC_StateManager::GetStateWithTag(const FGameplayTag& StateTag) const
{
	if (!StateTag.IsValid()) 
	{
		return nullptr;
	}

	// Find the instance of the requested state
	for (UStateBase* State : StateInstances)
	{
		if (State && State->StateTag == StateTag)
		{
			return State;
		}
	}

	return nullptr;
}






