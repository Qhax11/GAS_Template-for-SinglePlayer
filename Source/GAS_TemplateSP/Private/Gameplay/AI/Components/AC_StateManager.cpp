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
	FStateInitParams StateInitParams = FStateInitParams(OwnerEnemyBase, OwnerController, OwnerEnemyASC, BehaviorDecisionComponent, this);

	MovementState = Cast<UMovementState>(NewObject<UObject>(this, UMovementState::StaticClass()));
	MovementState->StateInitalize(StateInitParams);
	StateInstances.Add(MovementState);

	AttackState = Cast<UAttackState>(NewObject<UObject>(this, UAttackState::StaticClass()));
	AttackState->StateInitalize(StateInitParams);
	StateInstances.Add(AttackState);

	InComingAttackState = Cast<UInComingAttackState>(NewObject<UObject>(this, UInComingAttackState::StaticClass()));
	InComingAttackState->StateInitalize(StateInitParams);
	StateInstances.Add(InComingAttackState);
}

void UAC_StateManager::StartLogic()
{
	SelectNewBestAttack();
	EnterStateByClass(UMovementState::StaticClass());
}

void UAC_StateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		UE_LOG(LogTemp, Log, TEXT("CurrentState: %s"), *GetNameSafe(CurrentState));
		CurrentState->OnTick(DeltaTime);
	}
}

void UAC_StateManager::EnterStateByClass(TSubclassOf<UStateBase> StateClass)
{
	if (!StateClass)
	{
		return;
	}

	// Find the instance of the requested state
	for (UStateBase* State : StateInstances)
	{
		if (State && State->GetClass() == StateClass)
		{
			State->OnEnter();
			CurrentState = State;
			return;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("State class not found in StateInstances: %s"), *GetNameSafe(StateClass));
}

void UAC_StateManager::ExitStateByClass(TSubclassOf<UStateBase> StateClass)
{
	if (!StateClass)
	{
		return;
	}

	if (CurrentState)
	{
		CurrentState->OnExit();
		CurrentState = nullptr;
		return;
	}

	for (UStateBase* State : StateInstances)
	{
		if (State && State->GetClass() == StateClass)
		{
			CurrentState = State;
			CurrentState->OnExit();
			return;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("State class not found in StateInstances: %s"), *GetNameSafe(StateClass));
}

void UAC_StateManager::RequestStateTreeEnter(UStateBase* Requester, const FGameplayTag& TransactionTag)
{
	if (!Requester)
	{
		return;
	}

	if (Requester->IsA(UAttackState::StaticClass()))
	{
		EnterStateByClass(UAttackState::StaticClass());
	}
	else if (Requester->IsA(UInComingAttackState::StaticClass()))
	{
		EnterStateByClass(UInComingAttackState::StaticClass());
	}
	else if (Requester->IsA(UMovementState::StaticClass()))
	{
		EnterStateByClass(UMovementState::StaticClass());
	}
}

void UAC_StateManager::RequestStateTreeExit(UStateBase* Requester, const FGameplayTag& TransactionTag)
{
	if (!OwnerStateTree || !Requester)
	{
		return;
	}

	if (Requester->IsA(UAttackState::StaticClass()))
	{
		ExitFromAttackState(TransactionTag);
	}
	else if (Requester->IsA(UInComingAttackState::StaticClass()))
	{
		ExitFromInComingAttackState(TransactionTag);
	}
	else if (Requester->IsA(UMovementState::StaticClass()))
	{
		ExitFromMovementState(TransactionTag);
	}
}

void UAC_StateManager::ExitFromInComingAttackState(const FGameplayTag& TransactionTag)
{
	FAttackData NewSelectedAttack = SelectNewBestAttack();
	if (IsAttackInRange(NewSelectedAttack.AbilityClass))
	{
		OwnerStateTree->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_Transaction_AttackState_Enter);
	}
	else
	{
		OwnerStateTree->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_Transaction_MovementState_Enter);
	}
}

void UAC_StateManager::ExitFromAttackState(const FGameplayTag& TransactionTag)
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
		EnterStateByClass(UAttackState::StaticClass());
	}
	else
	{
		EnterStateByClass(UMovementState::StaticClass());
	}
	
}

void UAC_StateManager::ExitFromMovementState(const FGameplayTag& TransactionTag)
{
	if (TransactionTag == GAS_Tags::TAG_AI_StateTreeEvent_Transaction_AttackState_Enter)
	{
		EnterStateByClass(UAttackState::StaticClass());
	}
	else if (TransactionTag == GAS_Tags::TAG_AI_StateTreeEvent_Transaction_InComingAttackState_Enter) 
	{
		EnterStateByClass(UInComingAttackState::StaticClass());
	}
	else if (TransactionTag == GAS_Tags::TAG_AI_StateTreeEvent_Transaction_MovementState_Enter)
	{
		EnterStateByClass(UMovementState::StaticClass());
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






