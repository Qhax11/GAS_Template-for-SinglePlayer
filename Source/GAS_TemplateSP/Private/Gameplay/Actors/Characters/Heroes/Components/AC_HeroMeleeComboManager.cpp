// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"

UAC_HeroMeleeComboManager::UAC_HeroMeleeComboManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroMeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	HeroASC = HeroBase->GetAbilitySystemComponent();
	if (!HeroASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroASC is null in: %s"), *GetName());
		return;
	}

	HeroASC->OnAbilityEnded.AddUObject(this, &UAC_HeroMeleeComboManager::OnComboMeleeAttackAbilityEnd);
	BindHeroMeleeComboInput();
}

bool UAC_HeroMeleeComboManager::BindHeroMeleeComboInput()
{
	UAC_HeroControl* HeroControlComponent = HeroBase->GetHeroControlComponent();
	if (!HeroControlComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroControlComponent is null in: %s"), *GetName());
		return false;
	}

	UEnhancedInputComponent* EnhancedInputComponent = HeroControlComponent->GetEnhancedInputComponent();
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return false;
	}

	if (IA_ActivateMeleeCombo)
	{
		EnhancedInputComponent->BindAction(IA_ActivateMeleeCombo, ETriggerEvent::Triggered, this, &UAC_HeroMeleeComboManager::OnComboMeleeAttackInput);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
		return false;
	}
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackInput()
{
	ActivateComboMeleeAttackAbility();
}

void UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (HeroASC->HasAnyMatchingGameplayTags(BlockedTags)) 
	{
		return;
	}

	if (!HeroASC)
	{
		return;
	}

	if (!bCanActivateAbility)
	{
		return;
	}

	if (TSubclassOf<UGA_ComboMeleeAttack> ComboAbilityClass = GetNextComboMeleeAttackAbility())
	{
		if (FGameplayAbilitySpec* SpecHandle = HeroASC->FindAbilitySpecFromClass(ComboAbilityClass))
		{
			if (UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Cast<UGA_ComboMeleeAttack>(SpecHandle->GetPrimaryInstance()))
			{
				ActivatedComboMeleeAttack->SectionName = MontageSection;
				if (HeroASC->TryActivateAbilityByClass(ComboAbilityClass))
				{
					if (!ActivatedComboMeleeAttack->OnCanExecuteNextAttack.IsBound())
					{
						ActivatedComboMeleeAttack->OnCanExecuteNextAttack.AddDynamic(this, &UAC_HeroMeleeComboManager::OnCanActivateNextAttack);
					}
					bCanActivateAbility = false;
				}
			}
		}
	}
}


TSubclassOf<UGA_ComboMeleeAttack> UAC_HeroMeleeComboManager::GetNextComboMeleeAttackAbility()
{
	if (ComboMeleeAttackAbilities.IsValidIndex(AbilityIndex))
	{
		return ComboMeleeAttackAbilities[AbilityIndex++];
	}

	else if (AbilityIndex > ComboMeleeAttackAbilities.Num() - 1)
	{
		if (ComboMeleeAttackAbilities.IsValidIndex(0))
		{
			AbilityIndex = 0;
			return ComboMeleeAttackAbilities[AbilityIndex++];
		}
	}

	return nullptr;
}

void UAC_HeroMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	// If it is another ability or if it is UGA_HeroHologram we need a reset. 
	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>() || EndedData.AbilityThatEnded->IsA<UGA_HeroHologram>())
	{
		return;
	}

	// If ComboMelee ability is normal ended
	if (!EndedData.bWasCancelled)
	{
		AbilityIndex = 0;
	}

	bCanActivateAbility = true;

	OnComboMeleeEnded.Broadcast(EndedData.bWasCancelled);
}

void UAC_HeroMeleeComboManager::OnCanActivateNextAttack()
{
	bCanActivateAbility = true;
}



