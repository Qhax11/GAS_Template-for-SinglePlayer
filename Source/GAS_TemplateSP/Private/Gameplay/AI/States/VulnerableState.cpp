// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/VulnerableState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"


void UVulnerableState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	UGAS_GameplayAbilityBase* ActivatedHeroShadowFinisher = HeroTargetASC->TryActivateAbilityByClassAndReturnInstance(HeroShadowFinisherAbilityClass);
	if (ActivatedHeroShadowFinisher)
	{
		if (!ActivatedHeroShadowFinisher->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UVulnerableState::OnHeroShadowFinisherAbilityEnded))
		{
			ActivatedHeroShadowFinisher->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UVulnerableState::OnHeroShadowFinisherAbilityEnded);
		}
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVulnerableState::ActivateVulnerableAbility, Delay, false);
}

void UVulnerableState::ActivateVulnerableAbility()
{
	// If enemy before executed then timer we shouldn't activate vulnerable ability, because its interreptud dead ability.
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
	{
		return;
	}

	UGAS_GameplayAbilityBase* ActivatedVulnerableAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(VulnerableAbilityClass);
	if (ActivatedVulnerableAbility)
	{
		if (!ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UVulnerableState::OnVulnerableAbilityEnded))
		{
			ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UVulnerableState::OnVulnerableAbilityEnded);
		}

		LastUsedActivatedVulnerableAbility = ActivatedVulnerableAbility;
	}
}

void UVulnerableState::OnVulnerableAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead)) 
	{
		return;
	}
	else
	{
		ExitRequest("OnVulnerableAbilityEnded");
	}
}

void UVulnerableState::OnHeroShadowFinisherAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{

}

void UVulnerableState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedActivatedVulnerableAbility)
	{
		if (LastUsedActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UVulnerableState::OnVulnerableAbilityEnded))
		{
			LastUsedActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UVulnerableState::OnVulnerableAbilityEnded);
		}
		LastUsedActivatedVulnerableAbility = nullptr;
	}
}