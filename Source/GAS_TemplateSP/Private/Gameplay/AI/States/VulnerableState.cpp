// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/VulnerableState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"


void UVulnerableState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	UGAS_GameplayAbilityBase* ActivatedHeroShadowFinisher = HeroTargetASC->TryActivateAbilityByClassAndReturnInstance(HeroShadowFinisherAbilityClass);
	if (ActivatedHeroShadowFinisher)
	{
		ActivatedHeroShadowFinisher->OnAbilityEnded.RemoveAll(this);
		ActivatedHeroShadowFinisher->OnAbilityEnded.AddUObject(this, &UVulnerableState::OnHeroShadowFinisherAbilityEnded);
	}

	ActivateVulnerableAbility();
}

void UVulnerableState::ActivateVulnerableAbility()
{
	/*
	// If enemy before executed then timer we shouldn't activate vulnerable ability, because its interreptud dead ability.
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
	{
		return;
	}
	*/

	UGAS_GameplayAbilityBase* ActivatedVulnerableAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(VulnerableAbilityClass);
	if (ActivatedVulnerableAbility && ActivatedVulnerableAbility->IsActive())
	{
		ActivatedVulnerableAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedVulnerableAbility->OnAbilityEnded.AddUObject(this, &UVulnerableState::OnVulnerableAbilityEnded);
		LastUsedActivatedVulnerableAbility = ActivatedVulnerableAbility;
	}
	else
	{
		ExitRequest("Vulnerable ability couldn't executed");
	}
}

void UVulnerableState::OnVulnerableAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
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

void UVulnerableState::OnHeroShadowFinisherAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{

}

void UVulnerableState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedActivatedVulnerableAbility)
	{
		LastUsedActivatedVulnerableAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedActivatedVulnerableAbility = nullptr;
	}
}