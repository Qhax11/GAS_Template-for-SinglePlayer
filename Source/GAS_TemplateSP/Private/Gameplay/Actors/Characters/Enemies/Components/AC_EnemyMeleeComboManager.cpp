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

void UAC_EnemyMeleeComboManager::StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass)
{
	AbilityIndex = ComboMeleeAttackAbilities.Find(ComboMeleeAttackAbilityClass);
	ActivateComboMeleeAttackAbility();
}

void UAC_EnemyMeleeComboManager::ActivateComboMeleeAttackAbility(FName MontageSection)
{
	if (GetTargetDistance() < ComboRanges[AbilityIndex])
	{
		Super::ActivateComboMeleeAttackAbility();
	}
	// If target out of combo attack's range end combo
	else
	{
		OnComboEnded.Broadcast();
	}
}

void UAC_EnemyMeleeComboManager::OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData)
{
	Super::OnComboMeleeAttackAbilityEnd(EndedData);

	if (!EndedData.AbilityThatEnded->IsA<UGA_ComboMeleeAttack>())
	{
		return;
	}

	// This means we've reached the end of the combo
	if (AbilityIndex == ComboMeleeAttackAbilities.Num())
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
	if (!CharacterBase || !AIController || !AIController->Target)
	{
		return -1.f; 
	}

	FVector MyLocation = CharacterBase->GetActorLocation();
	FVector TargetLocation = AIController->Target->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}