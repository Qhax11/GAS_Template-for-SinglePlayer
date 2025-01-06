// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"

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

void UAC_MeleeComboManager::ActivateComboMeleeAttackAbility()
{
	if (!bCanActivateAbility) 
	{
		return;
	}

	if (TSubclassOf<UGA_ComboMeleeAttack> ComboAbilityClass = GetNextComboMeleeAttackAbility())
	{
		if (CharacterBaseASC->TryActivateAbilityByClass(ComboAbilityClass))
		{
			if (FGameplayAbilitySpec* SpecHandle = CharacterBaseASC->FindAbilitySpecFromClass(ComboAbilityClass))
			{
				if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(SpecHandle->GetPrimaryInstance()))
				{
					if (!ActivatedComboMeleeAttack->OnCanExecuteNextAttack.IsBound())
					{
						ActivatedComboMeleeAttack->OnCanExecuteNextAttack.AddDynamic(this, &UAC_MeleeComboManager::OnCanActivateNextAttack);
					}
					bCanActivateAbility = false;
				}
			}
		}
	}
}

TSubclassOf<UGA_ComboMeleeAttack> UAC_MeleeComboManager::GetNextComboMeleeAttackAbility()
{
	if (ComboMeleeAttackAbilities.IsValidIndex(AbilityIndex))
	{
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	else if (ComboMeleeAttackAbilities.IsValidIndex(0))
	{
		AbilityIndex = 0;
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	return nullptr;
}

void UAC_MeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	if (!EndedData.bWasCancelled)
	{
		AbilityIndex = 0;
	}
}

void UAC_MeleeComboManager::OnCanActivateNextAttack()
{
	bCanActivateAbility = true;
}


