// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"
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
	if(!ComboChain || !AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Attack: UAC_EnemyMeleeComboManager: ComboChain or AIController is null!"));
		return;
	}

	const FComboChainData& InComboChain = ComboChain->ComboChain;
	if (InComboChain.ComboAbilities.Num() == 0)
	{
		return;
	}

	ActiveComboChainTracker.StartChain(InComboChain);

	ActivateComboMelee();
}
 
UGA_ComboMeleeAttack* UAC_EnemyMeleeComboManager::ActivateComboMelee(const FComboPreActivationData& Data)
{
	const FComboAbilityData* CurrentCombo = ActiveComboChainTracker.GetCurrentCombo();
	if (!CurrentCombo || !CurrentCombo->ComboAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Attack: UAC_EnemyMeleeComboManager: CurrentCombo is null!"));
		return nullptr;
	}

	UGA_ComboMeleeAttack* DefaultAbilityCDO = CurrentCombo->ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>();
	if (!DefaultAbilityCDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Attack: UAC_EnemyMeleeComboManager: DefaultAbilityCDO is null!"));
		return nullptr;
	}

	float ComboAbilityMaxRange = DefaultAbilityCDO->MaxRange;
	if (AIController->GetTargetHeroDistance() < ComboAbilityMaxRange)
	{
		return Super::ActivateComboMelee(Data);
	}
	else
	{
		FinishComboChain(EEnemyComboChainResult::OutOfRange);
		return nullptr;
	}
}

void UAC_EnemyMeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& EndedData)
{
	if (EndedData.AbilityThatEnded != ActiveComboChainTracker.CurrentAbilityInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Attack: UAC_EnemyMeleeComboManager: Ended ComboAttack is not Current ComboAttack"), *EndedData.AbilityThatEnded->GetName());
		return;
	}

	if (EndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: ComboChain Cancelled"));
		if (CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage)) 
		{
			UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: Combo cancelled with HitTaken"));
			FinishComboChain(EEnemyComboChainResult::HitTaken);
		}
		else if (CharacterBaseASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry))
		{
			UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: Combo cancelled with ParryTriggered"));
			FinishComboChain(EEnemyComboChainResult::ParryTriggered);
		}
		return;
	}

	ActiveComboChainTracker.Advance();
	if (ActiveComboChainTracker.IsChainFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Attack:  UAC_EnemyMeleeComboManager: Combo Finished."));
		FinishComboChain(EEnemyComboChainResult::Completed);
		return;
	}

	// Continue next combo step
	ActivateComboMelee();
}

void UAC_EnemyMeleeComboManager::FinishComboChain(EEnemyComboChainResult Result)
{
	// Cache last ability BEFORE clearing
	UGameplayAbility* CachedLastAbility = ActiveComboChainTracker.CurrentAbilityInstance;

	// Clear internal state
	ClearComboChain();

	// Broadcast with cached data
	BroadcastComboChainEnd(CachedLastAbility, Result);
}

void UAC_EnemyMeleeComboManager::BroadcastComboChainEnd(UGameplayAbility* LastComboAbility, EEnemyComboChainResult Result)
{
	FEnemyComboChainEndData EnemyComboChainEndData;
	EnemyComboChainEndData.Result = Result;
	EnemyComboChainEndData.LastComboAbility = LastComboAbility;
	OnEnemyComboChainEnded.Broadcast(EnemyComboChainEndData);
}
