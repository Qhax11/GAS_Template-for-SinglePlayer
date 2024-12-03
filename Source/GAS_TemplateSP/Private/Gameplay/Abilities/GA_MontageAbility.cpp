// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

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
	
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTas = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SelectedMontage);
	PlayMontageAndWaitTas->OnBlendOut.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageAndWaitTas->OnCompleted.AddDynamic(this, &UGA_MontageAbility::OnMontageCompleted);
	PlayMontageAndWaitTas->OnInterrupted.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	PlayMontageAndWaitTas->OnCancelled.AddDynamic(this, &UGA_MontageAbility::OnMontageCancelled);
	//PlayMontageAndWaitTas->EventReceived.AddDynamic(this, &UGA_MontageAbility::OnEventReceived);
	PlayMontageAndWaitTas->ReadyForActivation();
}

UAnimMontage* UGA_MontageAbility::SelectSequence()
{
	return Montages.IsValidIndex(0) ? Montages[0] : nullptr;
}

void UGA_MontageAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_MontageAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_MontageAbility::OnEventReceived()
{
	/* Will be implemented in child classes */
}

