// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/VulnerableState.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"


void UVulnerableState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();


}

void UVulnerableState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVulnerableState::ActivateVulnerableAbility, 0.3f, false);

}

void UVulnerableState::ActivateVulnerableAbility()
{
	UGAS_GameplayAbilityBase* ActivatedVulnerableAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(VulnerableAbilityClass);
	if (ActivatedVulnerableAbility)
	{
		if (!ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UVulnerableState::OnVulnerableAbilityEnded))
		{
			ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UVulnerableState::OnVulnerableAbilityEnded);
		}
	}

	UGAS_GameplayAbilityBase* ActivatedHeroShadowFinisher = HeroTargetASC->TryActivateAbilityByClassAndReturnInstance(HeroShadowFinisherAbilityClass);
	if (ActivatedVulnerableAbility)
	{
		if (!ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UVulnerableState::OnHeroShadowFinisherAbilityEnded))
		{
			ActivatedVulnerableAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UVulnerableState::OnHeroShadowFinisherAbilityEnded);
		}
	}
	
}

void UVulnerableState::OnVulnerableAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{

}

void UVulnerableState::OnHeroShadowFinisherAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{

}
