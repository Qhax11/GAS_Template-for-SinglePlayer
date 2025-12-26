// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/AI/States/MovementState.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

UAC_StateManager::UAC_StateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_StateManager::BeginPlay()
{
	Super::BeginPlay();
} 

void UAC_StateManager::OnHeroSpawned(const FHeroSpawnData& HeroSpawnData)
{
	Super::OnHeroSpawned(HeroSpawnData);

	checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());
	checkf(OwnerEnemyBase, TEXT("OwnerEnemyBase is null in %s"), *GetClass()->GetName());
	checkf(OwnerEnemyASC, TEXT("OwnerEnemyASC is null in %s"), *GetClass()->GetName());
	checkf(HeroBase, TEXT("HeroBase is null in %s"), *GetClass()->GetName());
	checkf(HeroASC, TEXT("HeroASC is null in %s"), *GetClass()->GetName());

	//OwnerEnemyBase->GetAbilitySetComponent()->OnAbilitySetGiven.AddDynamic(this, &UAC_StateManager::OnAbilitySetGiven);

	BehaviorDecisionComponent = OwnerController->GetBehaviorDecisionComponent();
	checkf(BehaviorDecisionComponent, TEXT("BehaviorDecisionComponent is null in %s"), *GetClass()->GetName());

	EnemyTagDelegatesComponent = OwnerEnemyBase->GetTagDelegatesComponent();
	checkf(EnemyTagDelegatesComponent, TEXT("EnemyTagDelegatesComponent is null in %s"), *GetClass()->GetName());

	CreateStates();
	StartLogic();
}

void UAC_StateManager::OnAbilitySetGiven(const AActor* OwnerActor)
{
	/*
	CreateStates();
	StartLogic();
	*/
}

void UAC_StateManager::CreateStates()
{
	FStateInitParams StateInitParams;
	StateInitParams.Enemy = OwnerEnemyBase;
	StateInitParams.EnemyController = OwnerController;
	StateInitParams.EnemyASC = OwnerEnemyASC;
	StateInitParams.EnemyTagDelegatesComp = EnemyTagDelegatesComponent;
	StateInitParams.BehaviorDecisionComponent = BehaviorDecisionComponent;
	StateInitParams.HeroTarget = OwnerController->GetTargetActor();
	StateInitParams.HeroTargetASC = HeroASC;
	StateInitParams.StateManager = this;
	StateInitParams.MovementManager = OwnerEnemyBase->GetEnemyMovementManagerComponent();

	for (TSubclassOf<UStateBase> StateClass : StateClassArray)
	{
		if (!*StateClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("State: Manager: Invalid state class in array."));
			continue;
		}

		UStateBase* NewState = NewObject<UStateBase>(this, StateClass);
		if (!NewState || !NewState->StateTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("State: Manager: Failed to instantiate: %s"), *StateClass->GetName());
			continue;
		}

		NewState->StateInitalize(StateInitParams);
		NewState->OnStateTransitionRequested.AddUObject(this,&UAC_StateManager::HandleStateTransitionRequested);
		StateInstances.Add(NewState);
	}
}

void UAC_StateManager::StartLogic()
{
	RequestStateTreeEnter(StartState);
}

void UAC_StateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->OnTick(DeltaTime);
#if WITH_EDITOR
		if (bEnableDebug) 
		{
			const FVector Location = OwnerEnemyBase->GetActorLocation() + FVector(0.f, 0.f, 150.f);
			const FString DebugText = FString::Printf(TEXT("State: %s"), *CurrentState->GetName());
			DrawDebugString(GetWorld(), Location, DebugText, nullptr, FColor::Cyan, 0.f, true, 1.5f);
		}
#endif // WITH_EDITOR
	}
}

void UAC_StateManager::HandleIncomingEvent(const FGameplayTag& StateEventTag, TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!StateEventTag.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: StateEventTag is empty!"));
		return;
	}

	if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_TakeHit)
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_TakeHit, EnterPayload);
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_VulnerableTagAdded)
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Vulnerable, EnterPayload);
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_InComingAttack) 
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_InComingAttack, EnterPayload);
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_TargetDetected) 
	{
		DecideNextStateBasedOnAttackRange();
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_BackupReaction)
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_BackupReaction, EnterPayload);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: Unhandled StateEventTag: %s"), *StateEventTag.ToString());
	}
}

void UAC_StateManager::DecideNextStateBasedOnAttackRange()
{
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: BehaviorDecisionComponent is null!"));
		return;
	}

	UAttackDataBase* BestAttack = BehaviorDecisionComponent->GetBestAttack();
	if (!BestAttack) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: BestAttack is null!"));
		return;
	}

	const float MinRange = BestAttack->GetMinRange();
	const float MaxRange = BestAttack->GetMaxRange();
	const EMovementRangeResult RangeResult = CombatDistance::EvaluateAttackRange(OwnerEnemyBase, HeroBase, MinRange, MaxRange);
	if (RangeResult == EMovementRangeResult::InRange) 
	{
		TSharedPtr<FAttackStatePayload> AttackStatePayload = MakeShared<FAttackStatePayload>(BestAttack);
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Attack, AttackStatePayload);
	}
	else
	{
		UMovementChainData* BestMovementChainData = BehaviorDecisionComponent->GetBestMovementChain(BestAttack);
		TSharedPtr<FMovementStatePayload> MovementStatePayload = MakeShared<FMovementStatePayload>(BestMovementChainData, BestAttack);
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Movement, MovementStatePayload);
	}
}

void UAC_StateManager::HandleStateTransitionRequested(const FStateTransitionRequest& Request)
{
	if (!CurrentState)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: CurrentState is null."));
		return;
	}

	// Only active state can request transition
	if (CurrentState->StateTag != Request.SourceStateTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: RequestSourceState is not CurrentState, Request failed."));
		return;
	}

	// ExitCondition check BEFORE exiting
	if (!CurrentState->ExitCondition())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: Condition of %s is false, cannot exit"), *CurrentState->GetName());
		return;
	}

	UStateBase* ExitingState = CurrentState;

	// Important: set null BEFORE OnExit to avoid re-entrant callbacks treating it as current
	CurrentState = nullptr;
	ExitingState->OnExit();

	// If state suggested a target, try it first
	if (Request.TargetStateTag.IsValid())
	{
		if (RequestStateTreeEnter(Request.TargetStateTag, Request.Payload))
		{
			return;
		}
	}

	// Fallback decision
	DecideNextStateBasedOnAttackRange();
}

bool UAC_StateManager::RequestStateTreeEnter(const FGameplayTag& TargetStateTag, TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!TargetStateTag.IsValid() || !bActive)
	{
		return false;
	}

	if (bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: %s state has been requested to enter"), *TargetStateTag.ToString());
	}

	UStateBase* TargetState = GetStateWithTag(TargetStateTag);
	if (!IsValid(TargetState))
	{
		UE_LOG(LogTemp, Warning, TEXT("State: Manager: %s TargetState is null!"));
		return false;
	}

	if (TargetState->EnterCondition(EnterPayload))
	{
		if (CurrentState)
		{
			CurrentState->OnExit();
		}

		TargetState->OnEnter(EnterPayload);
		CurrentState = TargetState;
		return true;
	}
	else
	{
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("State: Manager: Condition of %s is false, cannot enter"), *TargetState->GetName());
		}
		return false;
	}
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

void UAC_StateManager::StopLogic()
{
	bActive = false;
}

void UAC_StateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	Super::EndPlay(EndPlayReason);
}


