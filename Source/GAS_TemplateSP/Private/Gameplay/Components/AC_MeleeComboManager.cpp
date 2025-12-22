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

	InitComboChainTracker(EComboType::GroundCombo);
}

void UAC_MeleeComboManager::InitComboChainTracker(EComboType NewComboType)
{
	if (!ComboChainAsset)
	{
		return;
	}

	// ---- Enum → Index translate ----
	int32 Index = static_cast<int32>(NewComboType);
	// EComboType::GroundCombo → 0
	// EComboType::AirCombo    → 1
	// EComboType::ShadowCombo → 2

	if (!ComboChainAsset->ComboChains.IsValidIndex(Index))
	{
		return;
	}

	// ---- Initialize tracker ----
	ActiveComboChainTracker.ComboChain = ComboChainAsset->ComboChains[Index];
	ActiveComboChainTracker.CurrentStepIndex = 0;

	ActiveComboChainTracker.CurrentAbilitySpecHandle = FGameplayAbilitySpecHandle();
	ActiveComboChainTracker.CurrentAbilityInstance = nullptr;
	ActiveComboChainTracker.CurrentAbilityClass = nullptr;

	// Pull first step
	const FComboAbilityData* FirstCombo = ActiveComboChainTracker.GetCurrentCombo();
	if (FirstCombo)
	{
		ActiveComboChainTracker.CurrentAbilityClass = FirstCombo->ComboAbilityClass;
	}
}

UGA_ComboMeleeAttack* UAC_MeleeComboManager::ActivateComboMelee(FName MontageSection, FGameplayTag AdditionalTag)
{
	if (!CharacterBaseASC)
	{
		return nullptr;
	}

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData || !ComboAbilityData->ComboAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: ComboAbilityData or ComboAbilityClass is null"));
		return nullptr;
	}

	// Get ability spec
	FGameplayAbilitySpec* AbilitySpec = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityData->ComboAbilityClass);
	if (!AbilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: AbilitySpec is null"));
		return nullptr;
	}

	// Add temp tag to help with activation filters
	AbilitySpec->DynamicAbilityTags.AddTag(AdditionalTag);

	// For PerActor instancing, set SectionName on PrimaryInstance BEFORE activation
	if (UGA_ComboMeleeAttack* PrimaryInstance = Cast<UGA_ComboMeleeAttack>(AbilitySpec->GetPrimaryInstance()))
	{
		PrimaryInstance->SectionName = MontageSection;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: TryActivate Ability: %s, index is: %d"), *ComboAbilityData->ComboAbilityClass->GetName(), ActiveComboChainTracker.CurrentStepIndex);

	// Try activate ability and get its instance
	UGA_ComboMeleeAttack* ActivatedAbility = Cast<UGA_ComboMeleeAttack>(
		CharacterBaseASC->TryActivateAbilityByClassAndReturnInstance(ComboAbilityData->ComboAbilityClass)
	);

	// Remove the tag AFTER activation attempt
	AbilitySpec->DynamicAbilityTags.RemoveTag(AdditionalTag);

	if (!ActivatedAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: ActivatedAbility is null"));
		return nullptr;
	}

	// Save handle and instance
	ActiveComboChainTracker.CurrentAbilitySpecHandle = AbilitySpec->Handle;
	ActiveComboChainTracker.CurrentAbilityInstance = ActivatedAbility;

	// Bind end event safely
	ActivatedAbility->OnAbilityEnded.RemoveAll(this);
	ActivatedAbility->OnAbilityEnded.AddUObject(this, &UAC_MeleeComboManager::OnComboAbilityEnd);
	UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: ActivatedAbility ability is binded: %s"), *ActivatedAbility->GetName());

	LastActivatedCombo = ActivatedAbility;

	return ActivatedAbility;
}

void UAC_MeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData)
{
	if (ComboAbilityEndedData.AbilityThatEnded)
	{
		ComboAbilityEndedData.AbilityThatEnded->OnAbilityEnded.RemoveAll(this);
	}

	if (LastActivatedCombo == ComboAbilityEndedData.AbilityThatEnded)
	{
		LastActivatedCombo = nullptr;
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
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeCombo);

	CharacterBaseASC->CancelAbilities(&CancelTags);
}

void UAC_MeleeComboManager::ContinueComboAfterCurrentStepEnded(bool bWasCancelled)
{
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished() || bWasCancelled)
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
		return;
	}

	ActivateComboMelee();
}

