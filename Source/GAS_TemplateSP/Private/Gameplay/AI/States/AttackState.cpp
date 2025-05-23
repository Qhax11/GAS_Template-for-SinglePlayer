// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UAttackState::OnComboChaindEnded); 
}

void UAttackState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack State has been enter"));
	if (!BehaviorDecisionComponent)
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : EnemyASC->GetActivatableAbilities())
	{
		if (Spec.Ability)
		{
			UE_LOG(LogTemp, Warning, TEXT("ASC has ability: %s"), *Spec.Ability->GetName());
		}
	}

	SelectedAttackClass = BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass;

	if (SelectedAttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		MakeComboAttack();
	}
	else if (SelectedAttackClass->IsChildOf(UGA_BossShadowAttack::StaticClass()))
	{
		MakeShadowAttack();
	}
	else
	{
		MakeAttack();
	}
}

void UAttackState::SelectedAttack()
{
}

void UAttackState::MakeAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(
			BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass);

	if (ActivatedAbility) 
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackState::OnAttackAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAttackState::OnAttackAbilityEnded);
		}
	}
}

void UAttackState::MakeComboAttack()
{
	UClass* AttackClass = BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass;

	if (AttackClass && AttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboAttackClass = AttackClass;
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboAttackClass);
	}
}

void UAttackState::MakeShadowAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);

	UGA_BossShadowAttack* ShadowAttack = Cast<UGA_BossShadowAttack>(ActivatedAbility);
	if (ShadowAttack)
	{
		if (!ShadowAttack->OnBossShadowAttackCompleted.IsAlreadyBound(this, &UAttackState::ExecuteShadowAttack))
		{
			ShadowAttack->OnBossShadowAttackCompleted.AddDynamic(this, &UAttackState::ExecuteShadowAttack);
		}

		if (!ShadowAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackState::OnShadowAttackAbilityEnded))
		{
			ShadowAttack->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAttackState::OnShadowAttackAbilityEnded);
		}
	}
}

void UAttackState::OnShadowAttackAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData)
{
	if (ShadowAttackAbilityEndedData.bWasCancelled) 
	{
		ExitRequest();
	}
}

void UAttackState::ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData)
{
	if (ShadowActorData.AbilityClass && ShadowActorData.AbilityClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboClass = TSubclassOf<UGA_ComboMeleeAttack>(ShadowActorData.AbilityClass);
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboClass, FName("Section2"));
	}
}

void UAttackState::OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest();
}

void UAttackState::OnComboChaindEnded()
{
	ExitRequest();
}
