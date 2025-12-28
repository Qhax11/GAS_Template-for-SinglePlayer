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
}

UGA_ComboMeleeAttack* UAC_MeleeComboManager::ActivateComboMelee(const FComboPreActivationData& Data)
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

	FGameplayAbilitySpec* AbilitySpec = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityData->ComboAbilityClass);
	if (!AbilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: AbilitySpec is null"));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: TryActivate Ability: %s, index is: %d"), *ComboAbilityData->ComboAbilityClass->GetName(), ActiveComboChainTracker.CurrentStepIndex);

	SetPreActivationData(AbilitySpec, Data);

	// Try activate ability and get its instance
	UGA_ComboMeleeAttack* ActivatedAbility = Cast<UGA_ComboMeleeAttack>(
		CharacterBaseASC->TryActivateAbilityByClassAndReturnInstance(ComboAbilityData->ComboAbilityClass));

	// Remove the tag AFTER activation attempt
	AbilitySpec->DynamicAbilityTags.RemoveTag(Data.AdditionalTag);

	if (!ActivatedAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: ActivatedAbility is null"));
		return nullptr;
	}

	// Save handle and instance
	ActiveComboChainTracker.CurrentAbilityInstance = ActivatedAbility;
	// Bind end event safely
	ActivatedAbility->OnAbilityEnded.RemoveAll(this);
	ActivatedAbility->OnAbilityEnded.AddUObject(this, &UAC_MeleeComboManager::OnComboAbilityEnd);
	UE_LOG(LogTemp, Warning, TEXT("[ComboMeleeAttack]: ActivatedAbility ability is binded: %s"), *ActivatedAbility->GetName());

	return ActivatedAbility;
}

void UAC_MeleeComboManager::SetPreActivationData(FGameplayAbilitySpec* AbilitySpec, const FComboPreActivationData& Data)
{
	// Add temp tag to help with activation filters
	AbilitySpec->DynamicAbilityTags.AddTag(Data.AdditionalTag);

	// For PerActor instancing, set SectionName on PrimaryInstance BEFORE activation
	if (UGA_ComboMeleeAttack* PrimaryInstance = Cast<UGA_ComboMeleeAttack>(AbilitySpec->GetPrimaryInstance()))
	{
		PrimaryInstance->SectionName = Data.ComboMontageSection;
		PrimaryInstance->SetPreActivationWarpTarget(Data.MotionWarpingLocation, Data.MotionWarpingRotation);
	}
}

void UAC_MeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& Data)
{
	if (Data.AbilityThatEnded)
	{
		Data.AbilityThatEnded->OnAbilityEnded.RemoveAll(this);
	}
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

void UAC_MeleeComboManager::ClearComboChain()
{
	ActiveComboChainTracker.CurrentAbilityInstance = nullptr;
	ActiveComboChainTracker.Reset();
}


