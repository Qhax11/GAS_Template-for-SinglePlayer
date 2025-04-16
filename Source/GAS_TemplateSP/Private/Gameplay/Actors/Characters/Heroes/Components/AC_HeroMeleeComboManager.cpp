// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Abilities/Hero/GA_HeroShadowAttack.h"

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

	InitComboChainTracker();
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

void UAC_HeroMeleeComboManager::OnComboMeleeAttackInput()
{
	ActivateComboMeleeAttackAbility();
}

UGA_ComboMeleeAttack* UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (CharacterBaseASC->HasAnyMatchingGameplayTags(BlockedTags))
	{
		return nullptr;
	}

	UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Super::ActivateComboMeleeAttackAbility(MontageSection);
	if (!ActivatedComboMeleeAttack) 
	{
		return nullptr;
	}

	if (!ActivatedComboMeleeAttack->OnCanExecuteNextAttack.IsBound())
	{
		ActivatedComboMeleeAttack->OnCanExecuteNextAttack.AddDynamic(this, &UAC_HeroMeleeComboManager::OnCanActivateNextAttack);
	}

	return ActivatedComboMeleeAttack;
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// If it is another ability or if it is UGA_HeroHologram we need a reset. 
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>() || EndedData.AbilityThatEnded->IsA<UGA_HeroShadowAttack>())
	{
		return;
	}

	if (EndedData.AbilitySpecHandle != ActiveComboChainTracker.CurrentAbilitySpecHandle)
	{
		return;
	}

	if (EndedData.bWasCancelled)
	{
		if (ActiveComboChainTracker.IsChainFinished())
		{
			ActiveComboChainTracker.Reset();
			OnComboEnded.Broadcast();
		}
	}
	// If ComboMelee ability ended as normal
	else if(!EndedData.bWasCancelled)
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}

	ActiveComboChainTracker.bNextAttackAllowed = true;
}

void UAC_HeroMeleeComboManager::OnCanActivateNextAttack()
{
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}





