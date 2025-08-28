// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_Attack.h"
#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"

void UBoss_State_Attack::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();
}

bool UBoss_State_Attack::SelectAndMakeAttack()
{
	SelectedAttackClass = GetSelectedAttackAbilityData().AbilityClass;
	if (!SelectedAttackClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedAttackClass is null in: %s"), *GetName());
		return false;
	}

	if (SelectedAttackClass->IsChildOf(UGA_BossShadowAttack::StaticClass()))
	{
		MakeShadowAttack();
		return true;
	}
	
	if (Super::SelectAndMakeAttack())
	{
		return true;
	}

	return false;
}

void UBoss_State_Attack::MakeShadowAttack()
{
	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);

	UGA_BossShadowAttack* ShadowAttack = Cast<UGA_BossShadowAttack>(ActivatedAbility);
	if (ShadowAttack)
	{
		if (!ShadowAttack->OnBossShadowAttackCompleted.IsAlreadyBound(this, &UBoss_State_Attack::ExecuteShadowAttack))
		{
			ShadowAttack->OnBossShadowAttackCompleted.AddDynamic(this, &UBoss_State_Attack::ExecuteShadowAttack);
		}

		if (!ShadowAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UBoss_State_Attack::OnShadowAttackAbilityEnded))
		{
			ShadowAttack->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UBoss_State_Attack::OnShadowAttackAbilityEnded);
		}

		LastUsedShadowAttack = ShadowAttack;
	}
}

void UBoss_State_Attack::OnShadowAttackAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData)
{
	// If it dosen't cancelled it's mean executed and we keep listening from executed side
	if (ShadowAttackAbilityEndedData.bWasCancelled)
	{
		ExitRequest("OnShadowAttackAbilityEnded");
	}
}

void UBoss_State_Attack::ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData)
{
	/*
	if (ShadowActorData.AbilityClass && ShadowActorData.AbilityClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboClass = TSubclassOf<UGA_ComboMeleeAttack>(ShadowActorData.AbilityClass);
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboClass, FName("Section2"));

		if (!Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UAttackState::OnComboChaindEnded))
		{
			Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UAttackState::OnComboChaindEnded);
		}
	}
	*/
}

void UBoss_State_Attack::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedShadowAttack)
	{
		if (LastUsedShadowAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UBoss_State_Attack::OnShadowAttackAbilityEnded))
		{
			LastUsedShadowAttack->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UBoss_State_Attack::OnShadowAttackAbilityEnded);
		}
		LastUsedShadowAttack = nullptr;
	}
}
