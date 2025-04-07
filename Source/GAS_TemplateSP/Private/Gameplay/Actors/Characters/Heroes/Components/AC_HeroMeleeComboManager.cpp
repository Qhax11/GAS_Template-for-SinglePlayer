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

void UAC_HeroMeleeComboManager::InitComboChainTracker()
{
	if (!ComboChainAsset || !ComboChainAsset->ComboChains.IsValidIndex(SelectedComboIndex))
	{
		return;
	}

	ActiveComboChainTracker.ComboChain = ComboChainAsset->ComboChains[SelectedComboIndex];
	ActiveComboChainTracker.CurrentIndex = 0;

	const FComboAbilityData* FirstCombo = ActiveComboChainTracker.GetCurrentCombo();
	if (FirstCombo)
	{
		ActiveComboChainTracker.CurrentAbilityClass = FirstCombo->ComboAbilityClass;
	}
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// If it is another ability or if it is UGA_HeroHologram we need a reset. 
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>() || EndedData.AbilityThatEnded->IsA<UGA_HeroShadowAttack>())
	{
		return;
	}

	if (EndedData.bWasCancelled)
	{
		if (ActiveComboChainTracker.IsChainFinished())
		{
			UE_LOG(LogTemp, Warning, TEXT("IsChainFinished with canceled"));
			ActiveComboChainTracker.Reset();
			OnComboEnded.Broadcast();
		}
	}
	// If ComboMelee ability is normal ended
	else if(!EndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsChainFinished without canceled"));
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}

	ActiveComboChainTracker.bNextAttackAllowed = true;
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


