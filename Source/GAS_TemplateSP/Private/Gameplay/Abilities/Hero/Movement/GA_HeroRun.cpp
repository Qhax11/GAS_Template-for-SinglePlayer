// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroRun.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"


void UGA_HeroRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CharacterBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (WaitRelease)
	{
		WaitRelease->OnRelease.AddDynamic(this, &UGA_HeroRun::OnInputReleased);
		WaitRelease->ReadyForActivation();
	}

	UGameplayEffect* GE_SpeedBoost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GE_SpeedBoostClass);
	GE_SpeedBoostHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(GE_SpeedBoost, 1, FGameplayEffectContextHandle());
}

void UGA_HeroRun::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_HeroRun::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitRelease && IsValid(WaitRelease))
	{
		WaitRelease->EndTask();
	}

	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_SpeedBoostHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
