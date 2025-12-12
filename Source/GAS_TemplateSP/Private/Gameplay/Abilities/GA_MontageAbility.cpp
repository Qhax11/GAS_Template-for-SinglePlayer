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

void UGA_MontageAbility::ActivateMotionWarping()
{
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s, ability cannot motion warping"), *GetName());
		return;
	}

	UMotionWarpingComponent* CharacterMotionWarpingComp = CharacterBase->GetMotionWarpingComponent();
	if (!CharacterMotionWarpingComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMotionWarpingComp is null in: %s, ability cannot motion warping"), *GetName());
		return;
	}

	FVector TargetLocation;
	bool bShouldUseTargetReach = false;

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
	{
		const AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
		if (Hero && Hero->GetTargetLockSystemComponent())
		{
			AActor* Target = Hero->GetTargetLockSystemComponent()->CurrentTarget;
			if (Target && bUseTargetReachDistance)
			{
				const FVector OwnerLocation = Hero->GetActorLocation();
				const float DistanceToTarget = FVector::Dist(OwnerLocation, Target->GetActorLocation());
				if (DistanceToTarget <= MaxRange)
				{
					bShouldUseTargetReach = true;
				}
			}
		}
	}
	else if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Enemy))
	{
		bShouldUseTargetReach = bUseTargetReachDistance;
	}

	if (bShouldUseTargetReach)
	{
		TargetLocation = CalculateReachLocationToTarget();
	}
	else
	{
		TargetLocation = CalculateMotionWarpingLocation();
	}

#if WITH_EDITOR
	if (bDebugPointMotionWarping)
	{
		DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 3);
	}
#endif

	CharacterMotionWarpingComp->AddOrUpdateWarpTargetFromLocation(MotionWarpingName, TargetLocation);
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

FVector UGA_MontageAbility::CalculateMotionWarpingLocation() const
{
	FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector Right = GetAvatarActorFromActorInfo()->GetActorRightVector();

	FVector TargetLocation = OwnerLocation;

	if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Forward)
	{
		TargetLocation += Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardLeft)
	{
		TargetLocation += Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_ForwardRight)
	{
		TargetLocation += Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Backward)
	{
		TargetLocation -= Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardLeft)
	{
		TargetLocation -= Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_BackwardRight)
	{
		TargetLocation -= Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Right)
	{
		TargetLocation += Right * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_Gameplay_Direction_Left)
	{
		TargetLocation -= Right * MotionWarpingDistance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown DirectionTag in: %s, defaulting to forward motion"), *GetName());
		TargetLocation += Forward * MotionWarpingDistance;
	}

	return TargetLocation;
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
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Startup);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_PostAttack);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_TakeHit);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Parry);
	TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Recovery);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTags(TagsToRemove);
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

