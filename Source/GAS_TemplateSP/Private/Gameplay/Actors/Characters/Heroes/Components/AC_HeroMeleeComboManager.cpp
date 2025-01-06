// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"

void UAC_HeroMeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(CharacterBase);
	if (HeroBase)
	{
		BindHeroMeleeComboInput();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}
}

bool UAC_HeroMeleeComboManager::BindHeroMeleeComboInput()
{
	UAC_HeroControl* HeroControlComponent = HeroBase->GetHeroControlComponent();
	if (!HeroControlComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroControlComponent is null in: %s"), *GetName());
		return false;
	}

	UEnhancedInputComponent* EnhancedInputComponent = HeroControlComponent->GetEnhancedInputComponent();
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return false;
	}

	if (IA_ActivateMeleeCombo)
	{
		EnhancedInputComponent->BindAction(IA_ActivateMeleeCombo, ETriggerEvent::Triggered, this, &UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
		return false;
	}
}

void UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility()
{
	if (CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram)) 
	{
		return;
	}

	Super::ActivateComboMeleeAttackAbility();
}
