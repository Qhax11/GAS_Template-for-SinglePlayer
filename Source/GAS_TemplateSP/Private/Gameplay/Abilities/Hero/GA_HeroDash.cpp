// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroDash.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Tags/GAS_Tags.h"


void UGA_HeroDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		// The melee combo ability plays a root motion montage, which conflicts with the ApplyRootMotion task.
		// Since two different root motion sources cannot be applied at the same time, we need to cancel the combo ability first.
		// This ensures that the root motion montage is no longer active before applying a new root motion task.
		GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelAbilityTags);
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}

}

FVector UGA_HeroDash::CalculateDestination()
{
	if (UAC_HeroControl* GetHeroControlComponent = HeroBase->GetHeroControlComponent())
	{
		FVector DashDirection = GetDashDirection(GetHeroControlComponent->LastMovementInput);
		FVector DashTargetLocation = DashDirection * DistanceMultiplier + HeroBase->GetActorLocation();
		return DashTargetLocation;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHeroControlComponent is null in: %s"), *GetName());
		return FVector::ZeroVector;
	}
}

FVector UGA_HeroDash::GetDashDirection(const FVector2D& LastMovementInput)
{
	// This is because when no input is provided, we want the dash to happen in the forward direction of the character
	if (LastMovementInput.X == 0 && LastMovementInput.Y == 0)
	{
		return GetAvatarActorFromActorInfo()->GetActorForwardVector();
	}

	// Else return the direction based on input
	return GetDirectionFromLastMovementInput(LastMovementInput);
}

FVector UGA_HeroDash::GetDirectionFromLastMovementInput(const FVector2D& LastMovementInput)
{
	FVector HeroForwardDirection = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector HeroRightDirection = GetAvatarActorFromActorInfo()->GetActorRightVector();

	// Create the direction vector based on input (using the forward and right directions)
	FVector Direction = HeroForwardDirection * LastMovementInput.Y + HeroRightDirection * LastMovementInput.X;

	return Direction;
}


