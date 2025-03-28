// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossDash.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

UGA_BossDash::UGA_BossDash()
{
	FinishVelocityMode = ERootMotionFinishVelocityMode::SetVelocity;
}

void UGA_BossDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	BossController = Cast<AAIControllerBase>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	if (!BossController)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossController is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	DashDirection = BossController->GetBehaviorDecisionComponent()->LastSelectedMovementyData.Direction;
	if (DashDirection == EMovementDirection::None) 
	{
		DashDirection = EMovementDirection::Forward;
	}
	
	// Set the final velocity after root motion ends to match the dash direction and magnitude.
    // This prevents a sudden stop and avoids unintended animation transitions caused by zero velocity.
	FinishSetVelocity = CalculateDestination();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_BossDash::CalculateDestination()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return FVector::ZeroVector;
	}

	FVector DirectionVector;

	switch (DashDirection)
	{
	case EMovementDirection::Forward:
		DirectionVector = AvatarActor->GetActorForwardVector();
		break;

	case EMovementDirection::Backward:
		DirectionVector = -AvatarActor->GetActorForwardVector();
		break;

	case EMovementDirection::Right:
		DirectionVector = AvatarActor->GetActorRightVector();
		break;

	case EMovementDirection::Left:
		DirectionVector = -AvatarActor->GetActorRightVector();
		break;

	default:
		DirectionVector = FVector::ZeroVector;
		break;
	}

	return AvatarActor->GetActorLocation() + DirectionVector * DistanceMultiplier;
}
