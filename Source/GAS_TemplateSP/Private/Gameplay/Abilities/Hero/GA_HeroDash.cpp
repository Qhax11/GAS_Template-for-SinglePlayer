// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroDash.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Tags/GAS_Tags.h"


void UGA_HeroDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		if (UAC_HeroControl* GetHeroControlComponent = HeroBase->GetHeroControlComponent())
		{
			FVector DashDirection = GetDashDirection(GetHeroControlComponent->LastMovementInput);

			FVector DashTargetLocation = DashDirection * DistanceMultiplier + HeroBase->GetActorLocation();

			UAbilityTask_ApplyRootMotionMoveToForce* DashRootMotionTask =
				UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
					this,
					TEXT("DashRootMotionTask"),
					DashTargetLocation,
					Duration,
					bSetNewMovementMode,
					NewMovementMode,
					bRestrictSpeedToExpected,
					DashCurve,
					FinishVelocityMode,
					FinishSetVelocity,
					FinishClampVelocity);

			if (DashRootMotionTask)
			{
				FGameplayTagContainer CancelAbilityTags;
				CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_MeleeCombo);
				HeroBase->GetAbilitySystemComponent()->CancelAbilities(&CancelAbilityTags);

				DashRootMotionTask->OnTimedOutAndDestinationReached.AddDynamic(this, &UGA_HeroDash::OnTaskTimedOut);
				DashRootMotionTask->ReadyForActivation();
					
				Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
			}
		}
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
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

void UGA_HeroDash::OnTaskTimedOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

