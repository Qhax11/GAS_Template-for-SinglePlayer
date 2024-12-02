// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Gameplay/Actors/PaperCharacters/GAS_PaperCharacterBase.h"


void UGA_MontageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAnimMontage* SelectedMontage = SelectSequence();
	if (!SelectedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedMontage is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	/*
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::UAbilityTask_PlayMontageAndWait(this, NAME_None, SelectedMontage, WaitForEventTag, Rate, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);
	Task->ReadyForActivation();
	*/
}

UAnimMontage* UGA_MontageAbility::SelectSequence()
{
	return Montages.IsValidIndex(0) ? Montages[0] : nullptr;
}

void UGA_MontageAbility::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	/* Will be implemented in child classes */

}

