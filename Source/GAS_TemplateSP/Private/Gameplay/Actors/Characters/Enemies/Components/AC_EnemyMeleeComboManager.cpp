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
}

void UAC_EnemyMeleeComboManager::StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass, FName MontageSection)
{
	if (!ComboChainAsset)
	{
		return;
	}

	FComboChainSearchResult SearchResult = GetComboChainOfSelectedComboAbility(ComboMeleeAttackAbilityClass);
	if (SearchResult.FindedComboIndex == INDEX_NONE)
	{
		return;
	}

	ActiveComboChainTracker.ComboChain = SearchResult.ComboChain;
	ActiveComboChainTracker.CurrentIndex = SearchResult.FindedComboIndex;

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData)
	{
		OnComboEnded.Broadcast();
		return;
	}

	ActivateComboMeleeAttackAbility(MontageSection);
}
 
void UAC_EnemyMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (GetTargetDistance() < ActiveComboChainTracker.GetCurrentCombo()->MaxRange)
	{
		Super::ActivateComboMeleeAttackAbility(MontageSection);
	}
	// If target out of combo attack's range end combo
	else
	{
		StopCombo();
		return;
	}
}

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	if (EndedData.AbilitySpecHandle != ActiveComboChainTracker.CurrentAbilitySpecHandle)
	{
		return;
	}

	// When the combo ability ends for any reason, we are able to trigger the next combo ability.
	ActiveComboChainTracker.bNextAttackAllowed = true;
	ActiveComboChainTracker.Advance();

	if (ActiveComboChainTracker.IsChainFinished())
	{
		OnComboEnded.Broadcast();
	}
	else
	{
		ActivateComboMeleeAttackAbility();
	}
}

float UAC_EnemyMeleeComboManager::GetTargetDistance()
{
	if (!CharacterBase || !AIController || !AIController->GetTarget())
	{
		return -1.f; 
	}

	FVector MyLocation = CharacterBase->GetActorLocation();
	FVector TargetLocation = AIController->GetTarget()->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}