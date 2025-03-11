// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"

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
		EnhancedInputComponent->BindAction(IA_ActivateMeleeCombo, ETriggerEvent::Triggered, this, &UAC_HeroMeleeComboManager::OnComboMeleeAttackInput);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
		return false;
	}
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// If it is another ability
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		// If it is UGA_HeroHologram we need a reset. 
		if (EndedData.AbilityThatEnded->IsA<UGA_HeroHologram>())
		{
			return;
		}
		// If it is not UGA_HeroHologram we need contiune the combo, so we dont reset.
		else
		{
			AbilityIndex = 0;
		}
	}

	// If ComboMelee ability is normal ended
	if (!EndedData.bWasCancelled)
	{
		AbilityIndex = 0;
	}

	bCanActivateAbility = true;

	OnComboMeleeEnded.Broadcast(EndedData);
}

void UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (CharacterBaseASC->HasAnyMatchingGameplayTags(BlockedTags)) 
	{
		return;
	}

	Super::ActivateComboMeleeAttackAbility(MontageSection);
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackInput()
{
	ActivateComboMeleeAttackAbility();
}
