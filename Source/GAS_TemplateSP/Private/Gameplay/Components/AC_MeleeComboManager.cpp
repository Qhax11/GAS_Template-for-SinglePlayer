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

	if (TSubclassOf<UGA_ComboMeleeAttack> ComboAbilityClass = ActiveComboChainTracker.GetCurrentCombo()->ComboAbilityClass)
	{
		if (FGameplayAbilitySpec* SpecHandle = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityClass))
		{
			if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(SpecHandle->GetPrimaryInstance()))
			{
				UE_LOG(LogTemp, Warning, TEXT("SelectedComboMeleeAttack: %s"), *ActivatedComboMeleeAttack->GetName());
				ActivatedComboMeleeAttack->SectionName = MontageSection;
				if (CharacterBaseASC->TryActivateAbilityByClass(ComboAbilityClass))
				{
					UE_LOG(LogTemp, Warning, TEXT("Activeted!"));
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

void UAC_MeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// If it is another ability. 
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	// When the combo ability ends for any reason, we are able to trigger the next combo ability.
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();
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
	UE_LOG(LogTemp, Warning, TEXT("Combo Index Advanced To: %d"), ActiveComboChainTracker.CurrentIndex);
}


