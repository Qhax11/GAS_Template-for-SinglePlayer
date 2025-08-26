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

		if (bDebugMotionWarping)
		{
			DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 3);
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
	// Eğer önceki task varsa onu temizle (montage da kesinlikle durmalı)
	if (PlayMontageWaitForEventTask)
	{
		PlayMontageWaitForEventTask->OnBlendOut.RemoveDynamic(this, &UGA_MontageAbility::OnMontageBlendOut);
		PlayMontageWaitForEventTask->OnCompleted.RemoveDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
		PlayMontageWaitForEventTask->OnInterrupted.RemoveDynamic(this, &UGA_MontageAbility::OnMontageInterrupted);
		PlayMontageWaitForEventTask->OnCancelled.RemoveDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
		PlayMontageWaitForEventTask->EventReceived.RemoveDynamic(this, &UGA_MontageAbility::OnEventReceived);

		PlayMontageWaitForEventTask->StopPlayingMontage();
		PlayMontageWaitForEventTask->EndTask();
		PlayMontageWaitForEventTask = nullptr;
	}

	// Yeni task oluştur
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
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnMontageInterrupted: AnimMontage is: %s"), *AnimMontage->GetName());
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnMontageCancelled: AnimMontage is: %s"), *AnimMontage->GetName());
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// TO DO: COMBO MANAGER İLE TAKE DAMAGE VE COMBOYU DİNLE BAŞKA HİÇBİR CALLBACK GEREK YOK. GEREKTİĞİNDE MANUEL CANCEL OLSUN
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
	UE_LOG(LogTemp, Warning, TEXT("%s' %s ability is ended"), *GetAvatarActorFromActorInfo()->GetName(), *GetName());
	if (PlayMontageWaitForEventTask)
	{
		PlayMontageWaitForEventTask->EndTask();
	}

	CleanupMotionWarping();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

