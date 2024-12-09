// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"


void UGA_MontageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* SelectedMontage = SelectSequence();
	if (!SelectedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedMontage is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CreatePlayMontageWaitForEvent(SelectedMontage);

	StartupEffects();
}

void UGA_MontageAbility::CreatePlayMontageWaitForEvent(UAnimMontage* Montage, FName SectionName)
{
	UGAS_Task_PlayMontageWaitForEvent* Task = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Montage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);
	Task->ReadyForActivation();
}

UAnimMontage* UGA_MontageAbility::SelectSequence()
{
	return Montages.IsValidIndex(0) ? Montages[0] : nullptr;
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

