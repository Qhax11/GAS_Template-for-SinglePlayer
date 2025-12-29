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

void UAC_HeroMeleeComboManager::ActivateComboMelee(const UComboPreActivationData* Data)
{
	Super::ActivateComboMelee(Data);
}

void UAC_HeroMeleeComboManager::OnComboAbilityActivated(UGA_ComboMeleeAttack* Instance)
{
	Super::OnComboAbilityActivated(Instance);
}

void UAC_HeroMeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& Data)
{
	Super::OnComboAbilityEnd(Data);

	UGAS_GameplayAbilityBase* Ability = Data.AbilityThatEnded;
	if (!Ability)
	{
		return;
	}

	// ?? SADECE aktif combo ability konuþabilir
	if (Ability != ActiveComboChainTracker.CurrentAbilityInstance)
	{
		return;
	}

	const bool bStillInCombo = CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo);

	// Normal end
	if (!Data.bWasCancelled)
	{
		FinishCombo();
		return;
	}

	// Combo ? combo geçiþi
	if (bStillInCombo)
	{
		if (bComboChainConsumed)
		{
			FinishCombo();
		}
		return;
	}

	// Dýþ interrupt
	FinishCombo();
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

void UAC_HeroMeleeComboManager::StartShadowCombo(const UComboPreActivationData* Data)
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
		bComboChainConsumed = true;
	}
}

void UAC_HeroMeleeComboManager::FinishCombo()
{
	bComboChainConsumed = false;
	ActiveComboChainTracker.CurrentAbilityInstance = nullptr;
	ActiveComboChainTracker.Reset();
	ChangeComboSet();
	OnComboEnded.Broadcast();
}

void UAC_HeroMeleeComboManager::OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChain(EHeroComboType::AirCombo);
}

void UAC_HeroMeleeComboManager::OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	InitComboChain(EHeroComboType::GroundCombo);
}






