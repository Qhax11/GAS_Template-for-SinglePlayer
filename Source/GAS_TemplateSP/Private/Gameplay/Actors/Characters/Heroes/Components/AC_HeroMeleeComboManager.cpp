// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Abilities/Hero/GA_HeroShadowAttack.h"

void UAC_HeroMeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(CharacterBase);
	if (HeroBase)
	{
		BindHeroMeleeComboInput();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	UAC_TagDelegates* HeroTagDelegatesComp = HeroBase->GetTagDelegatesComponent();
	if (!HeroTagDelegatesComp)
	{
		return;
	}

	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagAdded);
	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroMeleeComboManager::OnInAirTagRemoved);

	InitComboChainTracker();
}

bool UAC_HeroMeleeComboManager::BindHeroMeleeComboInput()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
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

UGA_ComboMeleeAttack* UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection, FGameplayTag AdditionalTag)
{
	if (CharacterBaseASC->HasAnyMatchingGameplayTags(BlockedTags))
	{
		return nullptr;
	}

	UGA_ComboMeleeAttack* ActivatedComboMeleeAttack = Super::ActivateComboMeleeAttackAbility(MontageSection, AdditionalTag);
	if (!ActivatedComboMeleeAttack) 
	{
		return nullptr;
	}

	if (!ActivatedComboMeleeAttack->OnCanExecuteNextAttack.IsBound())
	{
		ActivatedComboMeleeAttack->OnCanExecuteNextAttack.AddDynamic(this, &UAC_HeroMeleeComboManager::OnCanActivateNextAttack);
	}

	return ActivatedComboMeleeAttack;
}

void UAC_HeroMeleeComboManager::OnComboAbilityEnd(const FAbilityEndedDataBP& ComboAbilityEndedData)
{
	Super::OnComboAbilityEnd(ComboAbilityEndedData);

	// If it is another ability or if it is UGA_HeroHologram return. 
	if (!ComboAbilityEndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>() || ComboAbilityEndedData.AbilityThatEnded->IsA<UGA_HeroShadowAttack>())
	{
		return;
	}

	if (ComboAbilityEndedData.bWasCancelled)
	{
		if (!ActiveComboChainTracker.bNextAttackAllowed)
		{
			ActiveComboChainTracker.Reset();
			OnComboEnded.Broadcast();
		}
	}
	// If ComboMelee ability ended as normal
	else if (!ComboAbilityEndedData.bWasCancelled)
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}

	ActiveComboChainTracker.bNextAttackAllowed = true;
}

void UAC_HeroMeleeComboManager::OnCanActivateNextAttack()
{
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}

void UAC_HeroMeleeComboManager::ActivateComboMeleeAttackAbilityWithShadowAttack(FName MontageSection, int32 ComboIndex, FGameplayTag AdditionalTag)
{
	ActiveComboChainTracker.CurrentIndex = ComboIndex;
	ActivateComboMeleeAttackAbility(MontageSection, AdditionalTag);
}

void UAC_HeroMeleeComboManager::OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	// SelectedComboIndex = 1 means inair attack combo
	SelectedComboIndex = 1;
	InitComboChainTracker();
}

void UAC_HeroMeleeComboManager::OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	// SelectedComboIndex = 0 means land attack combo
	SelectedComboIndex = 0;
	InitComboChainTracker();
}





