// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"

void UStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Enemy = StateInitParams.Enemy;
	EnemyController = StateInitParams.EnemyController;
	EnemyASC = StateInitParams.EnemyASC;
	HeroTarget = StateInitParams.HeroTarget;
	BehaviorDecisionComponent = StateInitParams.BehaviorDecisionComponent;
	StateManager = StateInitParams.StateManager;
}

void UStateBase::ExitRequest(const FGameplayTag& TransactionTag)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	StateManager->RequestStateTreeExit(StateTag, TransactionTag);
}

bool UStateBase::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return false;
	}

	return StateManager->IsAttackInRange(AbilityClass);
}

FAttackData UStateBase::GetSelectedAttackAbilityData() const
{
	return StateManager->LastSelectedAttackData;
}

FComingAttackReactionData UStateBase::GetSelectedReactionData() const
{
	return StateManager->SelectedReactionData;
}

UGAS_GameplayAbilityBase* UStateBase::GetSelectedAttackAbilityCDO() const
{
	FAttackData SelectedAttackData = GetSelectedAttackAbilityData();
	return SelectedAttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
}

FAttackData UStateBase::SelectNewAttackAbility() const
{
	return StateManager->SelectNewBestAttack();
}

