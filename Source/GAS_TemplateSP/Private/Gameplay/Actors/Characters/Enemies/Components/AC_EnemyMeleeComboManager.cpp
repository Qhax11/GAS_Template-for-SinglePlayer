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
		// Geçerli bir combo zinciri bulundu.
	}

	// Yeni tracker baþlat
	ActiveComboChainTracker.ComboChain = SearchResult.ComboChain;
	ActiveComboChainTracker.CurrentIndex = SearchResult.FindedComboIndex;

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData)
	{
		OnComboEnded.Broadcast();
		return;
	}

	// Ability referansýný kaydet ve baþlat
	//ActiveComboChainTracker.CurrentAbility = ComboAbilityData->ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>();
	ActivateComboMeleeAttackAbility();
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
		OnComboEnded.Broadcast();
		ActiveComboChainTracker = FActiveComboChainTracker(); // Reset
		return;
	}

	Super::ActivateComboMeleeAttackAbility(MontageSection);
}

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	Super::OnComboMeleeAttackAbilityEnd(EndedData);

	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

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