// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

void UAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UAttackState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation(); 
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();
	SelectAndMakeAttack();
}

void UAttackState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (Enemy->GetEnemyMeleeComboManagerComponent()) 
	{
		if (Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UAttackState::OnComboChaindEnded))
		{
			Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.RemoveDynamic(this, &UAttackState::OnComboChaindEnded);
		}
	}
	
	if (LastUsedShadowAttack)
	{
		if (LastUsedShadowAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackState::OnShadowAttackAbilityEnded))
		{
			LastUsedShadowAttack->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UAttackState::OnShadowAttackAbilityEnded);
		}
		LastUsedShadowAttack = nullptr;
	}

	if (LastUsedAttack)
	{
		if (LastUsedAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackState::OnAttackAbilityEnded))
		{
			LastUsedAttack->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UAttackState::OnAttackAbilityEnded);
		}
		LastUsedAttack = nullptr;
	}
}

void UAttackState::SelectAndMakeAttack()
{
	SelectedAttackClass = GetSelectedAttackAbilityData().AbilityClass;

	if (SelectedAttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		MakeAttack();

		//MakeComboAttack();
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

void UAttackState::MakeAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(
			BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass);

	if (ActivatedAbility) 
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAttackState::OnAttackAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAttackState::OnAttackAbilityEnded);
		}

		LastUsedAttack = ActivatedAbility;
	}
}

void UAttackState::OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest("OnAttackAbilityEnded");
}

void UAttackState::MakeComboAttack()
{
	UClass* AttackClass = BehaviorDecisionComponent->LastSelectedAttackAbilityData.AbilityClass;

	if (AttackClass && AttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboAttackClass = AttackClass;
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboAttackClass);

		if (!Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UAttackState::OnComboChaindEnded))
		{
			Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UAttackState::OnComboChaindEnded);
		}
	}
}

void UAttackState::OnComboChaindEnded()
{
	ExitRequest("OnComboChaindEnded");
}

void UAttackState::MakeShadowAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);

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

		LastUsedShadowAttack = ShadowAttack;
	}
}

void UAttackState::OnShadowAttackAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData)
{
	// If it dosen't cancelled it's mean executed and we keep listening from executed side
	if (ShadowAttackAbilityEndedData.bWasCancelled) 
	{
		ExitRequest("OnShadowAttackAbilityEnded");
	}
}

void UAttackState::ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData)
{
	if (ShadowActorData.AbilityClass && ShadowActorData.AbilityClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboClass = TSubclassOf<UGA_ComboMeleeAttack>(ShadowActorData.AbilityClass);
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboClass, FName("Section2"));

		if (!Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UAttackState::OnComboChaindEnded))
		{
			Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UAttackState::OnComboChaindEnded);
		}
	}
}


