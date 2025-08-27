// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/GA_EnemyTakeDamage.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

UGA_EnemyTakeDamage::UGA_EnemyTakeDamage()
{
	CancelAbilitiesWithTag.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);
}

void UGA_EnemyTakeDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("StateManager: enemy take damage from: %s"), *TriggerEventData->Instigator->GetName());
	SetRotationToInstigator(TriggerEventData->Instigator);
}

void UGA_EnemyTakeDamage::SetRotationToInstigator(const AActor* Instigator)
{
	if (!Instigator)
	{
		return;
	}

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Avatar->GetActorLocation(), Instigator->GetActorLocation());

	// Z eksenine sabitle (dönme yukarýdan olmasýn)
	LookAtRot.Pitch = 0.f;
	LookAtRot.Roll = 0.f;

	// Her ikisini de güncelle
	Avatar->SetActorRotation(LookAtRot);

	if (APawn* Pawn = Cast<APawn>(Avatar))
	{
		if (AAIController* AIController = Cast<AAIController>(Pawn->GetController()))
		{
			AIController->SetControlRotation(LookAtRot);
		}
	}
}

void UGA_EnemyTakeDamage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
