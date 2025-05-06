// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_MeleeComboManager.h"

UAC_MeleeComboManager::UAC_MeleeComboManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_MeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	CharacterBase = Cast<AGAS_CharacterBase>(GetOwner());
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s)"), *GetName());
		return;
	}

	CharacterBaseASC = CharacterBase->GetAbilitySystemComponent();
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBaseASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}

	CharacterBaseASC->OnAbilityEnded.AddUObject(this, &UAC_MeleeComboManager::OnComboMeleeAttackAbilityEnd);
	InitComboChainTracker();
}

void UAC_MeleeComboManager::InitComboChainTracker()
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

UGA_ComboMeleeAttack* UAC_MeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection, FGameplayTag AdditionalTag)
{
	if (!CharacterBaseASC) 
	{
		return nullptr;
	}

	if (!ActiveComboChainTracker.bNextAttackAllowed)
	{
		return nullptr;
	}

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (ComboAbilityData && ComboAbilityData->ComboAbilityClass)
	{
		if (FGameplayAbilitySpec* AbilitySpec = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityData->ComboAbilityClass))
		{
			AbilitySpec->DynamicAbilityTags.AddTag(AdditionalTag);
			ActiveComboChainTracker.CurrentAbilitySpecHandle = AbilitySpec->Handle;
			ActiveComboChainTracker.CurrentAbilityInstance = AbilitySpec->GetPrimaryInstance();
			if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(AbilitySpec->GetPrimaryInstance()))
			{
				ActivatedComboMeleeAttack->SectionName = MontageSection;
				if (CharacterBaseASC->TryActivateAbilityByClass(ComboAbilityData->ComboAbilityClass))
				{
					AbilitySpec->DynamicAbilityTags.RemoveTag(AdditionalTag); 
					ActiveComboChainTracker.bNextAttackAllowed = false;
					return ActivatedComboMeleeAttack;
				}
			}
		}
	}

	return nullptr;
}

void UAC_MeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// Implementation will be in subclasses.
}

FComboChainSearchResult UAC_MeleeComboManager::GetComboChainOfSelectedComboAbility(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass)
{
	FComboChainSearchResult Result;

	if (!ComboChainAsset)
	{
		return Result;
	}

	for (int32 i = 0; i < ComboChainAsset->ComboChains.Num(); ++i)
	{
		const FComboChainData& ComboChainData = ComboChainAsset->ComboChains[i];

		for (int32 j = 0; j < ComboChainData.ComboAbilities.Num(); j++)
		{
			if (ComboChainData.ComboAbilities[j].ComboAbilityClass == ComboMeleeAttackAbilityClass)
			{
				Result.ComboChain = ComboChainData;
				Result.FindedComboIndex = j;
				return Result;
			}
		}
	}

	return Result;
}

void UAC_MeleeComboManager::StopCombo()
{
	CancelComboAbilities();
	ActiveComboChainTracker.Reset();
	OnComboEnded.Broadcast();
}

void UAC_MeleeComboManager::CancelComboAbilities()
{
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBaseASC is null in: %s"), *GetName());
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Attack_MeleeCombo);

	CharacterBaseASC->CancelAbilities(&CancelTags);
}
