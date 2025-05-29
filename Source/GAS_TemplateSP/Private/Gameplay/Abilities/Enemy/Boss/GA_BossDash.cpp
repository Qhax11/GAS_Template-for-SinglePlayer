// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossDash.h"

UGA_BossDash::UGA_BossDash()
{
	FinishVelocityMode = ERootMotionFinishVelocityMode::SetVelocity;

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dash;

	AbilityTriggers.Add(TriggerData);
}

void UGA_BossDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Attack);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Knocback);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Parry);
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_TakeDamage);
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelAbilityTags);
	
	// Set the final velocity after root motion ends to match the dash direction and magnitude.
    // This prevents a sudden stop and avoids unintended animation transitions caused by zero velocity.
	FinishSetVelocity = CalculateDestination();

	DashRootMotionTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
		this,
		TEXT("DashRootMotionTask"),
		CalculateDestination(),
		Duration,
		bSetNewMovementMode,
		NewMovementMode,
		bRestrictSpeedToExpected,
		DashCurve,
		FinishVelocityMode,
		FinishSetVelocity,
		FinishClampVelocity);

	BindRootMotionTask();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_BossDash::CalculateDestination()
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

	return AvatarActor->GetActorLocation() + DirectionVector * DistanceMultiplier;
}
