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

	CharacterBaseASC = Cast<UGAS_AbilitySystemComponent>(CharacterBase->GetAbilitySystemComponent());
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBaseASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}

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
	/*
	if (!ActiveComboChainTracker.bNextAttackAllowed)
	{
		return nullptr;
	}
	*/

	if (!CharacterBaseASC)
	{
		return nullptr;
	}

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData || !ComboAbilityData->ComboAbilityClass)
	{
		return nullptr;
	}

	// Get ability spec
	FGameplayAbilitySpec* AbilitySpec = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityData->ComboAbilityClass);
	if (!AbilitySpec)
	{
		return nullptr;
	}

	// Add temp tag to help with activation filters
	AbilitySpec->DynamicAbilityTags.AddTag(AdditionalTag);

	// For PerActor instancing, set SectionName on PrimaryInstance BEFORE activation
	if (UGA_ComboMeleeAttack* PrimaryInstance = Cast<UGA_ComboMeleeAttack>(AbilitySpec->GetPrimaryInstance()))
	{
		PrimaryInstance->SectionName = MontageSection;
	}

	// Try activate ability and get its instance
	UGA_ComboMeleeAttack* ActivatedAbility = Cast<UGA_ComboMeleeAttack>(
		CharacterBaseASC->TryActivateAbilityByClassAndReturnInstance(ComboAbilityData->ComboAbilityClass)
	);

	// Remove the tag AFTER activation attempt
	AbilitySpec->DynamicAbilityTags.RemoveTag(AdditionalTag);

	if (!ActivatedAbility)
	{
		return nullptr;
	}

	// Save handle and instance
	ActiveComboChainTracker.CurrentAbilitySpecHandle = AbilitySpec->Handle;
	ActiveComboChainTracker.CurrentAbilityInstance = ActivatedAbility;

	// Bind end event safely
	if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_MeleeComboManager::OnComboAbilityEnd))
	{
		ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAC_MeleeComboManager::OnComboAbilityEnd);
	}

	UE_LOG(LogTemp, Warning, TEXT("[StateManager]: Activated Combo Ability: %s"), *ComboAbilityData->ComboAbilityClass->GetName());

	ActiveComboChainTracker.bNextAttackAllowed = false;
	LastActivatedCombo = ActivatedAbility;

	return ActivatedAbility;
}

void UAC_MeleeComboManager::OnComboAbilityEnd(const FAbilityEndedDataBP& ComboAbilityEndedData)
{
	if (!LastActivatedCombo) 
	{
		return;
	}

	if (LastActivatedCombo->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_MeleeComboManager::OnComboAbilityEnd))
	{
		LastActivatedCombo->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UAC_MeleeComboManager::OnComboAbilityEnd);
	}
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
