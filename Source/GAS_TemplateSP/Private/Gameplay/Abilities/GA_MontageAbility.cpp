// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"


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

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
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
		FVector TargetLocation = StartLocation + (Forward * MotionWarpingForwardForce);

		CharacterMotionWarpingComp->AddOrUpdateWarpTargetFromLocation(MotionWarpingName, TargetLocation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMotionWarpingComp is null in: %s, ability cannot motion warping"), *GetName());
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

