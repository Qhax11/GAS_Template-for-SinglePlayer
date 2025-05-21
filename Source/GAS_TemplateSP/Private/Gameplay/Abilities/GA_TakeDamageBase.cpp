// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "AIController.h"

UGA_TakeDamageBase::UGA_TakeDamageBase()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_TakeDamage;

	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Dead);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
}

void UGA_TakeDamageBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s"), *GetName());
		return;
	}

	if (const UGA_MeleeAttackBase* MeleeAttackBase = Cast<UGA_MeleeAttackBase>(TriggerEventData->ContextHandle.GetAbility()))
	{
		AnimMontage = GetHitMontage(MeleeAttackBase->AnimMontage);
	}

	if (AGAS_EnemyBase* CharacterBase = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo()))
	{
		//CharacterBase->GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);

		CharacterBase->GetEnemyMeleeComboManagerComponent()->StopCombo();
	}

	// Using Motion Warping insted of this
	SetRotationToInstigator(TriggerEventData->Instigator);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UAnimMontage* UGA_TakeDamageBase::GetHitMontage(UAnimMontage* AttackMontage)
{
	if (AttackAndHitMontages.Contains(AttackMontage))
	{
		return AttackAndHitMontages[AttackMontage];
	}

	return nullptr;
}

void UGA_TakeDamageBase::SetRotationToInstigator(const AActor* Instigator)
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
