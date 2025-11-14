// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroRun.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"


UGA_HeroRun::UGA_HeroRun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Running);
}

void UGA_HeroRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UGameplayEffect* GE_SpeedBoost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GE_SpeedBoostClass);
	GE_SpeedBoostHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(GE_SpeedBoost, 1, FGameplayEffectContextHandle());
}

void UGA_HeroRun::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_SpeedBoostHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
