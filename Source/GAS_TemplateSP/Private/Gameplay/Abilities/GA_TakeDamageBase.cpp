// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Kismet/KismetMathLibrary.h"

UGA_TakeDamageBase::UGA_TakeDamageBase()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_Event_TakeDamage;

	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Dead);
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

	// Using Motion Warping insted of this
	//SetRotationToInstigator(TriggerEventData->Instigator);

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
		UE_LOG(LogTemp, Warning, TEXT("Instigator is null in: %s"), *GetName());
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), Instigator->GetActorLocation());
	GetAvatarActorFromActorInfo()->SetActorRotation(LookAtRotation);
}
