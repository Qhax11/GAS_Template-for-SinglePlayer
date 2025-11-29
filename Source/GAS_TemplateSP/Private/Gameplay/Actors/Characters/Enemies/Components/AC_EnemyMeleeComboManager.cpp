// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

void UAC_EnemyMeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIControllerBase>(CharacterBase->GetController());
	if (!AIController) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController is null in: %s"), *GetName());
		return;
	}

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		return;
	}

	EnemyTagDelegatesComp = CharacterBase->GetTagDelegatesComponent();
	if (!EnemyTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		return;
	}

}

void UAC_EnemyMeleeComboManager::StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass, FName MontageSection)
{
	if (!ComboChainAsset || !AIController)
	{
		return;
	}

	FComboChainSearchResult SearchResult = GetComboChainOfSelectedComboAbility(ComboMeleeAttackAbilityClass);
	if (SearchResult.FindedComboIndex == INDEX_NONE)
	{
		return;
	}

	ActiveComboChainTracker.ComboChain = SearchResult.ComboChain;
	ActiveComboChainTracker.CurrentStepIndex = SearchResult.FindedComboIndex;

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData)
	{
		OnComboEnded.Broadcast();
		return;
	}

	ActiveComboChainTracker.bIsActive = true;
	ActivateComboMeleeAttackAbility(MontageSection);
}
 
UGA_ComboMeleeAttack* UAC_EnemyMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection, FGameplayTag AdditionalTag)
{
	const FComboAbilityData* CurrentCombo = ActiveComboChainTracker.GetCurrentCombo();
	if (!CurrentCombo || !CurrentCombo->ComboAbilityClass)
	{
		StopCombo();
		return nullptr;
	}

	// Get default object safely
	UGA_ComboMeleeAttack* DefaultAbilityCDO = CurrentCombo->ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>();
	if (!DefaultAbilityCDO)
	{
		StopCombo();
		return nullptr;
	}

	// Now safe to read MaxRange
	float ComboAbilityMaxRange = DefaultAbilityCDO->MaxRange;

	// Check distance
	if (AIController->GetTargetHeroDistance() < ComboAbilityMaxRange)
	{
		return Super::ActivateComboMeleeAttackAbility(MontageSection);
	}
	else
	{
		StopCombo();
		return nullptr;
	}
}

void UAC_EnemyMeleeComboManager::OnEnemyCanActivateNextAttack()
{
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}

void UAC_EnemyMeleeComboManager::OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("[StateManager]: OnTakeDamageTagAdded: "));
}

void UAC_EnemyMeleeComboManager::OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("[StateManager]: OnTakeDamageTagRemovedFrom CombatManager: "));

	// If combo was active and our take damage ability is finished, we need continue.
	if (ActiveComboChainTracker.bIsActive) 
	{
		ActiveComboChainTracker.bNextAttackAllowed = true;
		ActiveComboChainTracker.Advance();
		ActivateComboMeleeAttackAbility();
	}

	EnemyTagDelegatesComp->UnregisterAllDelegatesForObject(this);
}

void UAC_EnemyMeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData)
{
	Super::OnComboAbilityEnd(ComboAbilityEndedData);

	// It is mean combo ability ended with take damage, we need listen end of it.
	bool OnTakeDamage = CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	if (OnTakeDamage) 
	{
		EnemyTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage, EListenMode::OnRemoved).BindDynamic(this, &UAC_EnemyMeleeComboManager::OnTakeDamageTagRemoved);
		return;
	}

	// When the combo ability ends for any reason, we are able to trigger the next combo ability.
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished() || ComboAbilityEndedData.bWasCancelled)
	{
		OnComboEnded.Broadcast();
		ActiveComboChainTracker.Reset();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[StateManager]: OnOwnerAbilityEnd: %s"), *ComboAbilityEndedData.AbilityThatEnded->GetName());
	ActivateComboMeleeAttackAbility();
}

float UAC_EnemyMeleeComboManager::GetMaxRangeOfCurrentAttack()
{
	const FComboAbilityData* CurrentComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (CurrentComboAbilityData->ComboAbilityClass)
	{
		return CurrentComboAbilityData->ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>()->MaxRange;
	}

	return -1.0f;
}

