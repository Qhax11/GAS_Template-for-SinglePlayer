// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"

void UStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	 Enemy = StateInitParams.Enemy;
    checkf(Enemy, TEXT("Enemy is null in %s"), *GetClass()->GetName());
    
    EnemyController = StateInitParams.EnemyController;
    checkf(EnemyController, TEXT("EnemyController is null in %s"), *GetClass()->GetName());
    
    EnemyASC = StateInitParams.EnemyASC;
    checkf(EnemyASC, TEXT("EnemyASC is null in %s"), *GetClass()->GetName());
    
    EnemyTagDelegatesComp = StateInitParams.EnemyTagDelegatesComp;
    checkf(EnemyTagDelegatesComp, TEXT("EnemyTagDelegatesComp is null in %s"), *GetClass()->GetName());
    
    BehaviorDecisionComponent = StateInitParams.BehaviorDecisionComponent;
    checkf(BehaviorDecisionComponent, TEXT("BehaviorDecisionComponent is null in %s"), *GetClass()->GetName());
    
    HeroTarget = StateInitParams.HeroTarget;
    checkf(HeroTarget, TEXT("HeroTarget is null in %s"), *GetClass()->GetName());
    
    HeroTargetASC = StateInitParams.HeroTargetASC;
    checkf(HeroTargetASC, TEXT("HeroTargetASC is null in %s"), *GetClass()->GetName());
    
    StateManager = StateInitParams.StateManager;
    checkf(StateManager, TEXT("StateManager is null in %s"), *GetClass()->GetName());

	MovementManager = StateInitParams.MovementManager;
	checkf(MovementManager, TEXT("MovementManager is null in %s"), *GetClass()->GetName());
}

void UStateBase::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UStateBase: StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UStateBase: StateManager: %s has been enter"), *StateTag.ToString());
	}
}

void UStateBase::OnExit_Implementation()
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UStateBase: StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UStateBase: %s has been exit"), *StateTag.ToString());
	}
}

void UStateBase::BroadcastTransition(const FGameplayTag& TargetStateTag, TSharedPtr<FStatePayloadBase> Payload, const FString& Reason)
{
    FStateTransitionRequest Request;
    Request.SourceStateTag = StateTag;
    Request.TargetStateTag = TargetStateTag;
    Request.Payload = Payload;

    UE_LOG(LogTemp, Warning, TEXT("State: UStateBase: %s has been BroadcastTransition, Reason is: %s"), *StateTag.ToString(), *Reason);

    OnStateTransitionRequested.Broadcast(Request);
}




