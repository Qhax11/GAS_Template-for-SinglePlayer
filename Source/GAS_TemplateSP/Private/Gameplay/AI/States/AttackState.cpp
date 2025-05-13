// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UAttackState::OnComboChaindEnded); 
}

void UAttackState::OnEnter()
{
	ExecuteSelectedAttack();
}

void UAttackState::ExecuteSelectedAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack State has been enter"));
	if (!BehaviorDecisionComponent)
	{
		return;
	}

	TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackClass = BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass;

	if (SelectedAttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		ExecuteComboAttack();
	}
	else if (SelectedAttackClass->IsChildOf(UGA_BossShadowAttack::StaticClass()))
	{
		ExecuteShadowAttack();
	}
	else
	{
		ExecuteAttack();
	}

}

void UAttackState::ExecuteAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(
			BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass);

	ActivatedAbility->OnGameplayAbilityEndedWithData.AddUObject(this, &UAttackState::OnAttackAbilityEnded);
}

void UAttackState::ExecuteComboAttack()
{
	UClass* AttackClass = BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass;

	if (AttackClass && AttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboAttackClass = AttackClass; 
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboAttackClass);
	}
}

void UAttackState::ExecuteShadowAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(
			BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass);

	ActivatedAbility->OnGameplayAbilityEndedWithData.AddUObject(this, &UAttackState::OnAttackAbilityEnded);
}

void UAttackState::OnAttackAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (!StateManager) 
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	StateManager->RequestStateTreeExit(this);
}

void UAttackState::OnComboChaindEnded()
{
	ExitRequest();
}
