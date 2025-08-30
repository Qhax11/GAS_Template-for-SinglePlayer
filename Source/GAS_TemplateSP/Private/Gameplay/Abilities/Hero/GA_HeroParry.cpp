// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UGA_HeroParry::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (WaitRelease)
	{
		WaitRelease->OnRelease.AddDynamic(this, &UGA_HeroParry::OnInputReleased);
		WaitRelease->ReadyForActivation();
	}
}

void UGA_HeroParry::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_HeroParry::OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(GetClass());
}


void UGA_HeroParry::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitRelease && IsValid(WaitRelease)) 
	{
		WaitRelease->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
