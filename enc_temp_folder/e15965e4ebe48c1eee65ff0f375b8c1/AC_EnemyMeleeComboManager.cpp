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

void UAC_EnemyMeleeComboManager::StartComboChain(UEnemyComboChainAsset* ComboChain, FName MontageSection)
{
	if(!ComboChain || AIController)
	{
		return;
	}

	const FComboChainData& InComboChain = ComboChain->ComboChain;
	if (InComboChain.ComboAbilities.Num() == 0)
	{
		return;
	}

	// Set tracker
	ActiveComboChainTracker.ComboChain = InComboChain;
	ActiveComboChainTracker.CurrentStepIndex = 0;

	// Validate step
	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData)
	{
		OnComboEnded.Broadcast();
		return;
	}

	// Start first attack
	ActivateComboMelee(MontageSection);
}
 
UGA_ComboMeleeAttack* UAC_EnemyMeleeComboManager::ActivateComboMelee(FName MontageSection, FGameplayTag AdditionalTag)
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
		return Super::ActivateComboMelee(MontageSection);
	}
	else
	{
		StopCombo();
		return nullptr;
	}
}

void UAC_EnemyMeleeComboManager::OnEnemyCanActivateNextAttack()
{
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		ActiveComboChainTracker.Reset();
		OnComboEnded.Broadcast();
	}
}

void UAC_EnemyMeleeComboManager::OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: OnTakeDamageTagAdded: "));
}

void UAC_EnemyMeleeComboManager::OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: OnTakeDamageTagRemovedFrom"));

	// If combo was active and our take damage ability is finished, we need continue.
	if (ActiveComboChainTracker.bIsActive) 
	{
		ActiveComboChainTracker.Advance();
		ActivateComboMelee();
	}

	EnemyTagDelegatesComp->UnregisterAllDelegatesForObject(this);
}

void UAC_EnemyMeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& EndedData)
{
	Super::OnComboAbilityEnd(EndedData);

	// It is mean combo ability ended with take damage, we need listen end of it.
	bool OnTakeDamage = CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	if (OnTakeDamage) 
	{
		EnemyTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage, EListenMode::OnRemoved).BindDynamic(this, &UAC_EnemyMeleeComboManager::OnTakeDamageTagRemoved);
		return;
	}

	ActiveComboChainTracker.CurrentAbilityInstance = nullptr;

	if (EndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: Combo cancelled by %s. Resetting."), *EndedData.AbilityThatEnded->GetName());
		BroadcastComboChainEnd(EEnemyComboChainResult::Cancelled);
		return;
	}

	// Advance combo
	ActiveComboChainTracker.Advance();

	// If its finished
	if (ActiveComboChainTracker.IsChainFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: Combo Finished."));
		BroadcastComboChainEnd(EEnemyComboChainResult::Completed);
		return;
	}
	// Continue next combo step
	else
	{
		ActivateComboMelee();
	}
}

void UAC_EnemyMeleeComboManager::BroadcastComboChainEnd(EEnemyComboChainResult Result)
{
	FEnemyComboChainEndData EnemyComboChainEndData;
	EnemyComboChainEndData.Result = Result;
	EnemyComboChainEndData.LastComboAbility = ActiveComboChainTracker.CurrentAbilityInstance;

	OnEnemyComboChainEnded.Broadcast(EnemyComboChainEndData);
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

