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
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);

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
		UE_LOG(LogTemp, Warning, TEXT("AnimMontage is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	CreatePlayMontageWaitForEvent();
}

void UGA_MontageAbility::CreatePlayMontageWaitForEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("State Manager: %s is will play."), *AnimMontage->GetName());

	PlayMontageWaitForEventTask = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(
		this, NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f);

	PlayMontageWaitForEventTask->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageBlendOut);
	PlayMontageWaitForEventTask->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageWaitForEventTask->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageInterrupted);
	PlayMontageWaitForEventTask->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	PlayMontageWaitForEventTask->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);

	PlayMontageWaitForEventTask->ReadyForActivation();
}

void UGA_MontageAbility::ActivateMotionWarping()
{
	if (!bEnableMotionWarping)
	{
		return;
	}

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s, ability cannot motion warping"), *GetName());
		return;
	}

	UMotionWarpingComponent* OwnerMotionWarping = CharacterBase->GetMotionWarpingComponent();
	if (!OwnerMotionWarping)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerMotionWarping is null in: %s, ability cannot motion warping"), *GetName());
		return;
	}

	FVector TargetLocation;
	bool bHasValidTarget = false;

	if (ShouldUseTargetReach())
	{
		bHasValidTarget = TryCalculateReachLocationToTarget(TargetLocation);
	}
	else
	{
		TargetLocation = CalculateMotionWarpingLocation();
		bHasValidTarget = !TargetLocation.IsNearlyZero();
	}

	if (!bHasValidTarget)
	{
		OwnerMotionWarping->RemoveWarpTarget(MotionWarpingName);
		return;
	}

	const FVector OwnerLocation = CharacterBase->GetActorLocation();

	// 🔴 SkewWarp safety guard
	constexpr float MinWarpDist = 5.f;
	if (FVector::DistSquared(OwnerLocation, TargetLocation) <= FMath::Square(MinWarpDist))
	{
		OwnerMotionWarping->RemoveWarpTarget(MotionWarpingName);
		return;
	}

	OwnerMotionWarping->AddOrUpdateWarpTargetFromLocation(MotionWarpingName, TargetLocation);
}

bool UGA_MontageAbility::ShouldUseTargetReach() const
{
	if (!bUseTargetReachDistance)
	{
		return false;
	}

	AActor* Target = GetCurrentTargetActor();
	if (!Target)
	{
		return false;
	}

	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float Distance = FVector::Dist(OwnerLocation, Target->GetActorLocation());

	return Distance <= MaxRange;
}

bool UGA_MontageAbility::TryCalculateReachLocationToTarget(FVector& OutTargetLocation) const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	AActor* Target = GetCurrentTargetActor();

	if (!Avatar || !Target)
	{
		return false;
	}

	const FVector OwnerLocation = Avatar->GetActorLocation();
	const FVector ToTarget = Target->GetActorLocation() - OwnerLocation;

	const float Distance = ToTarget.Size();
	if (Distance <= KINDA_SMALL_NUMBER)
	{
		return false;
	}

	if (Distance > TargetReachDistance)
	{
		OutTargetLocation = Target->GetActorLocation() -
			ToTarget.GetSafeNormal() * TargetReachDistance;
		return true;
	}

	return false;
}

FVector UGA_MontageAbility::CalculateMotionWarpingLocation() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return FVector::ZeroVector;
	}

	const FVector OwnerLocation = Avatar->GetActorLocation();
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector Right = Avatar->GetActorRightVector();

	FVector Direction = Forward;

	if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardLeft)
		Direction = (Forward - Right).GetSafeNormal();
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardRight)
		Direction = (Forward + Right).GetSafeNormal();
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Backward)
		Direction = -Forward;
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardLeft)
		Direction = (-Forward - Right).GetSafeNormal();
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardRight)
		Direction = (-Forward + Right).GetSafeNormal();
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Left)
		Direction = -Right;
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Right)
		Direction = Right;

	return OwnerLocation + Direction * MotionWarpingDistance;
}

FVector UGA_MontageAbility::CalculateReachLocationToTarget() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return FVector::ZeroVector;
	}

	const FVector OwnerLocation = Avatar->GetActorLocation();

	AActor* Target = nullptr;

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
	{
		const AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(Avatar);
		if (Hero && Hero->GetTargetLockSystemComponent())
		{
			Target = Hero->GetTargetLockSystemComponent()->CurrentTarget;
		}
	}

	else if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Enemy))
	{
		const AGAS_EnemyBase* Enemy = Cast<AGAS_EnemyBase>(Avatar);
		if (Enemy && Enemy->GetEnemyController())
		{
			Target = Enemy->GetEnemyController()->GetTargetActor();
		}
	}

	if (!Target)
	{
		return OwnerLocation;
	}

	const FVector ToTarget = Target->GetActorLocation() - OwnerLocation;
	const float Distance = ToTarget.Size();

	if (Distance > TargetReachDistance)
	{
		return Target->GetActorLocation() - ToTarget.GetSafeNormal() * TargetReachDistance;
	}

	return OwnerLocation;
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
			? Hero->GetTargetLockSystemComponent()->CurrentTarget
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

void UGA_MontageAbility::CleanupMotionWarping()
{
	if (!bEnableMotionWarping)
	{
		return;
	}

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s, ability cannot CleanupMotionWarping"), *GetName());
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
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Startup);
	PhaseTagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);
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
		if (bEnableMotionWarping)
		{
			ActivateMotionWarping();
		}
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

