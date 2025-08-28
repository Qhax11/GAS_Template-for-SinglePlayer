// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

UAttackStateBase::UAttackStateBase()
{
	StateTag = GAS_Tags::TAG_AI_State_Attack;
}

void UAttackStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UAttackStateBase::OnEnter_Implementation()
{
	Super::OnEnter_Implementation(); 
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();
	SelectAndMakeAttack();
}

bool UAttackStateBase::SelectAndMakeAttack()
{
	SelectedAttackClass = GetSelectedAttackAbilityData().AbilityClass;
	if (!SelectedAttackClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedAttackClass is null in: %s"), *GetName());
		return false;
	}

	MakeAttack();
	return true;
}

void UAttackStateBase::MakeAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(
			BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass);

	if (ActivatedAbility) 
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackStateBase::OnAttackAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAttackStateBase::OnAttackAbilityEnded);
		}

		LastUsedAttack = ActivatedAbility;
	}
}

void UAttackStateBase::OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest("OnAttackAbilityEnded");
}

void UAttackStateBase::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedAttack)
	{
		if (LastUsedAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackStateBase::OnAttackAbilityEnded))
		{
			LastUsedAttack->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UAttackStateBase::OnAttackAbilityEnded);
		}
		LastUsedAttack = nullptr;
	}
}