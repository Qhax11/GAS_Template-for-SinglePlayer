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
		UE_LOG(LogTemp, Warning, TEXT("UAC_HeroMeleeComboManager is null in: %s"), *GetName());
		return;
	}

	UAC_TagDelegates* HeroTagDelegatesComp = HeroBase->GetTagDelegatesComponent();
	if (!HeroTagDelegatesComp)
	{
		return;
	}

	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroMeleeComboManager::OnPhaseActiveHitTagAdded);
	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagAdded);
	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagRemoved);

	InitComboChain(EHeroComboType::GroundCombo);
}

void UAC_HeroMeleeComboManager::InitComboChain(EHeroComboType ComboType)
{
	if (!HeroComboAsset)
	{
		return;
	}

	// Reset tracker
	ActiveComboChainTracker.Reset();

	// Set new chain based on type
	switch (ComboType)
	{
	case EHeroComboType::GroundCombo:
		ActiveComboChainTracker.ComboChain = HeroComboAsset->GroundCombo;
		break;
	case EHeroComboType::AirCombo:
		ActiveComboChainTracker.ComboChain = HeroComboAsset->AirCombo;
		break;
	case EHeroComboType::ShadowCombo:
		ActiveComboChainTracker.ComboChain = HeroComboAsset->ShadowCombo;
		break;
	}

	ActiveComboChainTracker.CurrentStepIndex = 0;
}

UGA_ComboMeleeAttack* UAC_HeroMeleeComboManager::ActivateComboMelee(const FComboPreActivationData& Data)
{
	UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Super::ActivateComboMelee(Data);
	if (!ActivatedComboMeleeAttack) 
	{
		return nullptr;
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

	UE_LOG(LogTemp, Warning, TEXT("UAC_HeroMeleeComboManager: OnComboAbilityEnd: %s"), *ComboAbilityEndedData.AbilityThatEnded->GetName());

	if (ComboAbilityEndedData.bWasCancelled)
	{
		// It is mean Interrupted by any ability that doesen't combo, so we need a reset
		if (!CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo))
		{
			UE_LOG(LogTemp, Warning, TEXT("UAC_HeroMeleeComboManager: Combo Interreptued from non-combo ability"));
			ActiveComboChainTracker.Reset();
			OnComboEnded.Broadcast();
		}
	}
	// If ComboMelee ability ended as normal
	else if(!ComboAbilityEndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_HeroMeleeComboManager: ability ended as normal!"));
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}

void UAC_HeroMeleeComboManager::ChangeComboSet()
{
	if (CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InAir))
	{
		InitComboChain(EHeroComboType::AirCombo);
	}
	else
	{
		InitComboChain(EHeroComboType::GroundCombo);
	}
}

void UAC_HeroMeleeComboManager::StartShadowCombo(const FComboPreActivationData& Data)
{
	InitComboChain(EHeroComboType::ShadowCombo);
	ActivateComboMelee(Data);
}

void UAC_HeroMeleeComboManager::OnPhaseActiveHitTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	// We need listen only combo abilitie's PhaseActiveHit Tag
	if (!CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo))
	{
		return;
	}

	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ChangeComboSet();
		OnComboEnded.Broadcast();
	}
}

void UAC_HeroMeleeComboManager::OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChain(EHeroComboType::AirCombo);
}

void UAC_HeroMeleeComboManager::OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChain(EHeroComboType::GroundCombo);
}






