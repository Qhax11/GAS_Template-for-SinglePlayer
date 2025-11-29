// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Abilities/Hero/GA_HeroShadowAttack.h"

void UAC_HeroMeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(CharacterBase);
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	UAC_TagDelegates* HeroTagDelegatesComp = HeroBase->GetTagDelegatesComponent();
	if (!HeroTagDelegatesComp)
	{
		return;
	}

	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagAdded);
	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagRemoved);

	InitComboChainTracker(EComboType::GroundCombo);

	CharacterBaseASC->AbilityActivatedCallbacks.AddUObject(this, &UAC_HeroMeleeComboManager::OnHeroAbilityActivated);
}

void UAC_HeroMeleeComboManager::OnHeroAbilityActivated(UGameplayAbility* Ability)
{
	if (!Ability)
	{
		return;
	}

	/*
	if (!Ability->IsA<UGA_ComboMeleeAttack>() || !Ability->IsA<UGA_HeroShadowAttack>())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
	*/
}

UGA_ComboMeleeAttack* UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection, FGameplayTag AdditionalTag)
{
	UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Super::ActivateComboMeleeAttackAbility(MontageSection, AdditionalTag);
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

void UAC_HeroMeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData)
{
	Super::OnComboAbilityEnd(ComboAbilityEndedData);

	// If it is another ability or if it is UGA_HeroHologram return. 
	if (!ComboAbilityEndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>() || ComboAbilityEndedData.AbilityThatEnded->IsA<UGA_HeroShadowAttack>())
	{
		return;
	}

	// It is mean Interrupted by any ability that doesen't combo, so we need a reset
	if (ComboAbilityEndedData.bWasCancelled)
	{
		if (!CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo))
		{
			ActiveComboChainTracker.Reset();
			OnComboEnded.Broadcast();
		}
	}
	// If ComboMelee ability ended as normal
	else
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
		ChangeComboSet();
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}

void UAC_HeroMeleeComboManager::ChangeComboSet()
{
	if (CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InAir))
	{
		InitComboChainTracker(EComboType::AirCombo);
	}
	else
	{
		InitComboChainTracker(EComboType::GroundCombo);
	}
}

void UAC_HeroMeleeComboManager::StartShadowCombo(FName MontageSection, FGameplayTag AdditionalTag)
{
	InitComboChainTracker(EComboType::ShadowCombo);
	ActivateComboMeleeAttackAbility(MontageSection, AdditionalTag);
}

void UAC_HeroMeleeComboManager::OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChainTracker(EComboType::AirCombo);
}

void UAC_HeroMeleeComboManager::OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChainTracker(EComboType::GroundCombo);
}





