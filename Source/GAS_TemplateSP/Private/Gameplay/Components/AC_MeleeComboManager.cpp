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
}

void UAC_MeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (!CharacterBaseASC) 
	{
		return;
	}

	if (!ActiveComboChainTracker.bNextAttackAllowed)
	{
		return;
	}

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (ComboAbilityData && ComboAbilityData->ComboAbilityClass)
	{
		if (FGameplayAbilitySpec* SpecHandle = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityData->ComboAbilityClass))
		{
			ActiveComboChainTracker.CurrentAbilitySpecHandle = SpecHandle->Handle;
			ActiveComboChainTracker.CurrentAbilityInstance = SpecHandle->GetPrimaryInstance(); 
			if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(SpecHandle->GetPrimaryInstance()))
			{
				ActivatedComboMeleeAttack->SectionName = MontageSection;
				if (CharacterBaseASC->TryActivateAbilityByClass(ComboAbilityData->ComboAbilityClass))
				{
					if (!ActivatedComboMeleeAttack->OnCanExecuteNextAttack.IsBound())
					{
						ActivatedComboMeleeAttack->OnCanExecuteNextAttack.AddDynamic(this, &UAC_MeleeComboManager::OnCanActivateNextAttack);
					}
					ActiveComboChainTracker.bNextAttackAllowed = false;
				}
			}
		}
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
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_MeleeCombo);

	CharacterBaseASC->CancelAbilities(&CancelTags);
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

		for (const FComboAbilityData& ComboAbilityData : ComboChainData.ComboAbilities)
		{
			if (ComboAbilityData.ComboAbilityClass == ComboMeleeAttackAbilityClass)
			{
				Result.ComboChain = ComboChainData;
				Result.FindedComboIndex = i;
				return Result;
			}
		}
	}

	return Result;
}

void UAC_MeleeComboManager::OnCanActivateNextAttack()
{
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}


