// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossDodge.h"

UGA_BossDodge::UGA_BossDodge()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dash;

	AbilityTriggers.Add(TriggerData);
}

void UGA_BossDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (TriggerEventData->InstigatorTags.IsValidIndex(0)) 
	{
		DirectionTag = TriggerEventData->InstigatorTags.GetByIndex(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no direction tag in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	FGameplayTagContainer CancelAbilityTags;
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Knocback);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Parry);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_TakeDamage);
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelAbilityTags);
	

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_BossDodge::CalculateDestination()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return FVector::ZeroVector;
	}

	FVector DirectionVector = FVector::ZeroVector;

	if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Forward)
	{
		DirectionVector = AvatarActor->GetActorForwardVector();
	}
	else if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Backward)
	{
		DirectionVector = -AvatarActor->GetActorForwardVector();
	}
	else if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Left)
	{
		DirectionVector = -AvatarActor->GetActorRightVector();
	}
	else if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Right)
	{
		DirectionVector = AvatarActor->GetActorRightVector();
	}

	return AvatarActor->GetActorLocation() + DirectionVector;
}
