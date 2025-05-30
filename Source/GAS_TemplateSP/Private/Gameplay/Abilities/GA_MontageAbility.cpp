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

	if (bEnableMotionWarping) 
	{
		ActivateMotionWarping();
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
		UE_LOG(LogTemp, Warning, TEXT("CleanupMotionWarping in %s"), *GetName());
		MotionWarping->RemoveWarpTarget(MotionWarpingName); 
		//MotionWarping->RemoveAllWarpTargets();// veya ClearWarpTargets() kullanabilirsin
	}
}

void UGA_MontageAbility::CreatePlayMontageWaitForEvent()
{
	UGAS_Task_PlayMontageWaitForEvent* Task = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);
	Task->ReadyForActivation();
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	/* Will be implemented in child classes */
}

void UGA_MontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanupMotionWarping();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

