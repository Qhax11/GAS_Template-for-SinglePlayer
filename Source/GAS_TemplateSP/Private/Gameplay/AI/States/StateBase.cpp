// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"

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
}

void UStateBase::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[StateManager]: %s has been enter"), *StateTag.ToString());
	}
}

void UStateBase::OnExit_Implementation()
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[StateManager]: %s has been exit"), *StateTag.ToString());
	}
}

bool UStateBase::ExitRequest(FString Reason, FStateTransitionRequest StateTransitionRequest)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return false; 
	}

	return StateManager->RequestStateTreeExit(StateTransitionRequest, Reason);
}

bool UStateBase::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return false;
	}

	return BehaviorDecisionComponent->IsAttackInRange(AbilityClass);
}

FAttackData UStateBase::GetSelectedAttackAbilityData() const
{
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return FAttackData();
	}

	return BehaviorDecisionComponent->LastSelectedAttackData;
}

UGAS_GameplayAbilityBase* UStateBase::GetSelectedAttackAbilityCDO() const
{
	FAttackData SelectedAttackData = GetSelectedAttackAbilityData();
	return SelectedAttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
}




