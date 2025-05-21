// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/States/AttackState.h"
#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "StateTreeExecutionContext.h"


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

	CreateStates();
}

void UAC_StateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState) 
	{
		CurrentState->OnTick(DeltaTime);
	}
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
			CurrentState = State;
			CurrentState->OnEnter();
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

void UAC_StateManager::RequestStateTreeExit(UStateBase* Requester)
{
	if (!OwnerStateTree || !Requester)
	{
		return;
	}

	FGameplayTag ExitEventTag;

	if (Requester->IsA(UAttackState::StaticClass()))
	{
		ExitEventTag = GAS_Tags::TAG_AI_StateTreeEvent_Transaction_AttackState_Exit;
	}
	else if (Requester->IsA(UInComingAttackState::StaticClass()))
	{
		ExitEventTag = GAS_Tags::TAG_AI_StateTreeEvent_Transaction_InComingAttackState_Exit;
	}
	else if (Requester->IsA(UMovementState::StaticClass()))
	{
		ExitEventTag = GAS_Tags::TAG_AI_StateTreeEvent_Transaction_MovementState_Exit;
	}

	if (ExitEventTag.IsValid())
	{
		OwnerStateTree->SendStateTreeEvent(ExitEventTag);
	}
}

void UAC_StateManager::StopCurrentState()
{
}

bool UAC_StateManager::IsCurrentStateFinished() const
{
	return false;
}




