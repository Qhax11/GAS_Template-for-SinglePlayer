// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include <Abilities/Tasks/AbilityTask_WaitDelay.h>
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"


UGA_MontageAbility::UGA_MontageAbility()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Dodge);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Parry);

	WaitForEventTag.AddTag(GAS_Tags::TAG_Gameplay_Event_AnimNotify_Movement_MotionWarping);
}

void UGA_MontageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
 	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (!OwnerActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!AnimMontage && !IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_MontageAbility: AnimMontage is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	CreatePlayMontageWaitForEvent();
}

void UGA_MontageAbility::CreatePlayMontageWaitForEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_MontageAbility: %s is will play."), *AnimMontage->GetName());

	PlayMontageWaitForEventTask = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(
		this, NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, 1.0f);

	PlayMontageWaitForEventTask->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageBlendOut);
	PlayMontageWaitForEventTask->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageWaitForEventTask->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageInterrupted);
	PlayMontageWaitForEventTask->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	PlayMontageWaitForEventTask->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);

	PlayMontageWaitForEventTask->ReadyForActivation();
}

void UGA_MontageAbility::TryActivateMotionWarping()
{
	if (WarpTargetMode == EWarpTargetMode::None || !CharacterBase)
	{
		return;
	}

	UMotionWarpingComponent* MotionWarpingComp = CharacterBase->GetMotionWarpingComponent();
	if (!MotionWarpingComp)
	{
		return;
	}

	FVector Location;
	FRotator Rotation;

	if (!TryBuildWarpTarget(Location, Rotation))
	{
		MotionWarpingComp->RemoveWarpTarget(MotionWarpingName);
		return;
	}

	if (!IsWarpDistanceValid(Location))
	{
		MotionWarpingComp->RemoveWarpTarget(MotionWarpingName);
		return;
	}

#if WITH_EDITOR
	DebugDrawWarpTarget(Location);
#endif // WITH_EDITOR

	// For right now, we just use rotation warping with PreActivation.
	if (WarpTargetMode == EWarpTargetMode::PreActivation)
	{
		// ❗ PreActivation datası YOKSA warp yapma
		if (PreActivationWarpLocation.IsNearlyZero())
		{
			return;
		}

		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(MotionWarpingName, Location, Rotation);
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("UGA_MontageAbility: PreActivation WarpTarget | Name=%s | Location=%s | Rotation=%s | in this: %s"),
			*MotionWarpingName.ToString(),
			*Location.ToString(),
			*Rotation.ToString(),
			*GetName()
		);
	}
	else
	{
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(MotionWarpingName, Location);
	}
}

bool UGA_MontageAbility::TryBuildWarpTarget(FVector& OutLocation, FRotator& OutRotation)
{
	if (WarpTargetMode == EWarpTargetMode::Directional) 
	{
		UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: WarpTargetMode is Directional in this: %s"), *GetName());
		OutLocation = CalculateDirectionalWarpLocation();
		return true;
	}
	else if (WarpTargetMode == EWarpTargetMode::TargetReach)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: TargetReach is Directional in this: %s"), *GetName());
		bool ValidReachLocation = TryCalculateReachLocationToTarget(OutLocation);
		if (!ValidReachLocation) 
		{
			UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: InValid ReachLocation, try with CalculateDirectionalWarpLocation(), in this: %s"), *GetName());
			OutLocation = CalculateDirectionalWarpLocation();
		}

		return false;
	}
	else if (WarpTargetMode == EWarpTargetMode::PreActivation)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: TargetReach is PreActivation in this: %s"), *GetName());
		OutLocation = PreActivationWarpLocation;
		OutRotation = PreActivationWarpRotation;
		return true;
	}

	return false;
}

FVector UGA_MontageAbility::CalculateDirectionalWarpLocation() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return FVector::ZeroVector;
	}

	const FVector OwnerLocation = Avatar->GetActorLocation();
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector Right = Avatar->GetActorRightVector();

	FVector DesiredDirection;

	if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Forward)
	{
		DesiredDirection = Forward;
	}
	if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardLeft)
	{
		DesiredDirection = (Forward - Right).GetSafeNormal();
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardRight)
	{
		DesiredDirection = (Forward + Right).GetSafeNormal();
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Backward) 
	{
		DesiredDirection = -Forward;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardLeft)
	{
		DesiredDirection = (-Forward - Right).GetSafeNormal();
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardRight)
	{
		DesiredDirection = (-Forward + Right).GetSafeNormal();
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Left)
	{
		DesiredDirection = -Right;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Right)
	{
		DesiredDirection = Right;
	}

	return OwnerLocation + DesiredDirection * MotionWarpingDistance;
}

bool UGA_MontageAbility::TryCalculateReachLocationToTarget(FVector& OutTargetLocation) const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	AActor* Target = GetCurrentTargetActor();

	if (!IsValid(Avatar) || !IsValid(Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_MontageAbility: TargetReach: Target or Avatar is invalid! in this: %s"), *GetName());
		return false;
	}

	const FVector OwnerLocation = Avatar->GetActorLocation();
	const FVector ToTarget = Target->GetActorLocation() - OwnerLocation;

	const float Distance = ToTarget.Size();
	if (Distance <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_MontageAbility: TargetReach: Distance is so low in this: %s"), *GetName());
		return false;
	}

	if (Distance > MaxRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_MontageAbility: TargetReach: Distance is bigger than MaxRange! in this: %s"), *GetName());
		return false;
	}

	if (Distance > TargetReachDistance)
	{
		OutTargetLocation = Target->GetActorLocation() - ToTarget.GetSafeNormal() * TargetReachDistance;
		UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: TargetReach: Moving closer to target, Distance = %f"), Distance);
		return true;
	}
	else
	{
		OutTargetLocation = OwnerLocation;
		UE_LOG(LogTemp, Log, TEXT("Ability: UGA_MontageAbility: TargetReach: TargetReach Already close, staying in place, Distance=%f"), Distance);
		return true;
	}
}

AActor* UGA_MontageAbility::GetCurrentTargetActor() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return nullptr;
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
		GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
	{
		const AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(Avatar);
		return Hero && Hero->GetTargetLockSystemComponent()
			? Hero->GetTargetLockSystemComponent()->GetCurrentTarget()
			: nullptr;
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
		GAS_Tags::TAG_Gameplay_Entity_Character_Enemy))
	{
		const AGAS_EnemyBase* Enemy = Cast<AGAS_EnemyBase>(Avatar);
		return Enemy && Enemy->GetEnemyController()
			? Enemy->GetEnemyController()->GetTargetActor()
			: nullptr;
	}

	return nullptr;
}

void UGA_MontageAbility::SetPreActivationWarpTarget(const FVector& InLocation, const FRotator& InRotation)
{
	PreActivationWarpLocation = InLocation;
	PreActivationWarpRotation = InRotation;
}

bool UGA_MontageAbility::IsWarpDistanceValid(const FVector& TargetLocation) const
{
	constexpr float MinWarpDist = 10.f;
	const FVector OwnerLocation = CharacterBase->GetActorLocation();
	return FVector::DistSquared(OwnerLocation, TargetLocation) > FMath::Square(MinWarpDist);
}

#if WITH_EDITOR
void UGA_MontageAbility::DebugDrawWarpTarget(const FVector& TargetLocation) const
{
	if (bDebugPointMotionWarping)
	{
		DrawDebugPoint(
			GetWorld(),
			TargetLocation,
			15.f,
			FColor::Red,
			false,
			2.f);
	}
}
#endif // WITH_EDITOR

void UGA_MontageAbility::CleanupMotionWarping()
{
	if (WarpTargetMode == EWarpTargetMode::None || !CharacterBase)
	{
		return;
	}

	if (UMotionWarpingComponent* MotionWarping = CharacterBase->GetMotionWarpingComponent())
	{
		MotionWarping->RemoveWarpTarget(MotionWarpingName); 
	}
}

void UGA_MontageAbility::CleanupPhaseTags()
{
	FGameplayTagContainer PhaseTagsToRemove;
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Startup);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_PostAttack);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_TakeHit);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Parry);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Dodge);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Recovery);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTags(PhaseTagsToRemove);
}

void UGA_MontageAbility::EndAbilityManually()
{
	MontageStop(0.0f);
	Super::EndAbilityManually();
}

void UGA_MontageAbility::HandleMontageEvent(bool bWasCancelled)
{
	if (MontageEndPolicy == EMontageEndPolicy::Never)
	{
		return;
	}

	CancelBlendOutDelay();

	if (bWasCancelled)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	if (MontageEndPolicy == EMontageEndPolicy::EndWithDelay)
	{
		WaitForBlendOutAndEnd(bWasCancelled);
	}
	else if (MontageEndPolicy == EMontageEndPolicy::Standard)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, bWasCancelled);
	}
}

void UGA_MontageAbility::OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(false);
}

void UGA_MontageAbility::WaitForBlendOutAndEnd(bool bWasCancelled)
{
	CancelBlendOutDelay();

	if (AnimMontage && AnimMontage->BlendOut.GetBlendTime() > 0.1f )
	{
		// Animation Blend Time Correction: 
		// Visually, the character appears to have transitioned to the next pose (e.g., Idle) 
		// before the engine's internal blend timer fully expires. 
		// Subtracting a small, empirical value corrects this perceptual delay, preventing 
		// the ability from unnecessarily delaying player input execution (input lag).
		float const BlendDuration = AnimMontage->BlendOut.GetBlendTime() - 0.1f;
		BlendOutDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, BlendDuration);

		if (BlendOutDelayTask)
		{
			BlendOutDelayTask->OnFinish.AddDynamic(this, &UGA_MontageAbility::OnBlendOutDelayFinished);
			BlendOutDelayTask->ReadyForActivation();
		}
	}
	else
	{
		// No blend time, end immediately
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, bWasCancelled);
	}
}

void UGA_MontageAbility::CancelBlendOutDelay()
{
	if (IsValid(BlendOutDelayTask))
	{
		if (BlendOutDelayTask->IsActive())
		{
			BlendOutDelayTask->EndTask();
		}
		BlendOutDelayTask = nullptr;
	}
}

void UGA_MontageAbility::OnBlendOutDelayFinished()
{
	BlendOutDelayTask = nullptr;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(false);
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(true);
}

void UGA_MontageAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Purpose: Activates Motion Warping to align the character precisely with the target position/rotation at the specific moment determined by the animation.
	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Movement_MotionWarping)
	{
		TryActivateMotionWarping();
	}
}

void UGA_MontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Delay task cleanup
	CancelBlendOutDelay();

	if (PlayMontageWaitForEventTask && IsValid(PlayMontageWaitForEventTask))
	{
		if (PlayMontageWaitForEventTask->IsActive())
		{
			PlayMontageWaitForEventTask->EndTask();
		}
		PlayMontageWaitForEventTask = nullptr;
	}

	CleanupPhaseTags();

	CleanupMotionWarping();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


