// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"


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

	if (!AnimMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimMontage is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	CreatePlayMontageWaitForEvent();

	UE_LOG(LogTemp, Warning, TEXT("Ability is triggered: %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("AnimMontage is: %s"), *AnimMontage->GetName());
}

void UGA_MontageAbility::ActivateMotionWarping()
{
	AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s, ability cannot motion warping"), *GetName());
		return;
	}

	if (UMotionWarpingComponent* CharacterMotionWarpingComp = CharacterBase->GetMotionWarpingComponent())
	{
		FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
		FVector StartLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		FVector TargetLocation = StartLocation;

		// Determine direction
		if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Forward)
		{
			// Move forward
			TargetLocation += Forward * MotionWarpingForce;
			if (bDebugMotionWarping) 
			{
				DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 3);
			}
		}
		else if (DirectionTag == GAS_Tags::TAG_AI_Direction_Resolved_Backward)
		{
			// Move backward
			TargetLocation -= Forward * MotionWarpingForce;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unknown DirectionTag in: %s, defaulting to forward motion"), *GetName());
			TargetLocation += Forward * MotionWarpingForce;
		}

		CharacterMotionWarpingComp->AddOrUpdateWarpTargetFromLocation(MotionWarpingName, TargetLocation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMotionWarpingComp is null in: %s, ability cannot motion warping"), *GetName());
	}
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
	PlayMontageWaitForEventTask = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f);
	PlayMontageWaitForEventTask->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageWaitForEventTask->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageWaitForEventTask->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	PlayMontageWaitForEventTask->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	PlayMontageWaitForEventTask->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);
	PlayMontageWaitForEventTask->ReadyForActivation();
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("OnMontageCancelled: AnimMontage is: %s"), *AnimMontage->GetName());
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_MotionWarping)
	{
		if (bEnableMotionWarping)
		{
			ActivateMotionWarping();
		}
	}
}

void UGA_MontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (PlayMontageWaitForEventTask) 
	{
		PlayMontageWaitForEventTask->EndTask();
	}

	CleanupMotionWarping();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

