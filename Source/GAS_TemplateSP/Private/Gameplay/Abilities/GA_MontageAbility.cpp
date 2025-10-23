// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

UGA_MontageAbility::UGA_MontageAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
	AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
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
	if (bUseDestinationReachForDistance) 
	{
		TargetLocation = CalculateDestinationReachLocation();
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

FVector UGA_MontageAbility::CalculateDestinationReachLocation() const
{
	FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	AGAS_EnemyBase* Enemy = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo());
	if (!Enemy) 
	{
		return OwnerLocation;
	} 

	AAIControllerBase* EnemyController = Enemy->GetEnemyController();
	if (!EnemyController)
	{
		return OwnerLocation;
	}

	AActor* TargetHero = EnemyController->GetTargetActor();
	if (!TargetHero)
	{
		return OwnerLocation;
	}

	FVector ToTarget = TargetHero->GetActorLocation() - OwnerLocation;
	float Distance = ToTarget.Size();
	if (Distance > DestinationReachDistance)
	{
		return TargetHero->GetActorLocation() - ToTarget.GetSafeNormal() * DestinationReachDistance;
	}

	return OwnerLocation;
}

FVector UGA_MontageAbility::CalculateMotionWarpingLocation() const
{
	FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector TargetLocation = OwnerLocation;

	if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Forward)
	{
		TargetLocation += Forward * MotionWarpingDistance;
	}
	else if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Backward)
	{
		TargetLocation -= Forward * MotionWarpingDistance;
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

	AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!CharacterBase)
	{
		return;
	}

	if (UMotionWarpingComponent* MotionWarping = CharacterBase->GetMotionWarpingComponent())
	{
		MotionWarping->RemoveWarpTarget(MotionWarpingName); 
	}
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

void UGA_MontageAbility::OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnMontageBlendOut: AnimMontage is: %s"), *AnimMontage->GetName());
	if (MontageEndPolicy == EMontageEndPolicy::Any || MontageEndPolicy == EMontageEndPolicy::BlendOut)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UGA_MontageAbility::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnMontageInterrupted: AnimMontage is: %s"), *AnimMontage->GetName());
	if (MontageEndPolicy == EMontageEndPolicy::Any || MontageEndPolicy == EMontageEndPolicy::Interrupted)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (MontageEndPolicy == EMontageEndPolicy::Any || MontageEndPolicy == EMontageEndPolicy::Completed)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UGA_MontageAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_MotionWarping)
	{
		if (bEnableMotionWarping)
		{
			ActivateMotionWarping();
		}
	}
}

void UGA_MontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (PlayMontageWaitForEventTask && IsValid(PlayMontageWaitForEventTask))
	{
		PlayMontageWaitForEventTask->EndTask();
		PlayMontageWaitForEventTask = nullptr;
	}

	CleanupMotionWarping();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

