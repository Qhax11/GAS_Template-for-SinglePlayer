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

	FGameplayEffectSpec PostureDecreaseSpec;
	bool bIsPostureRegenSpecValid = UGAS_EffectBlueprintFunctionLibary::CreateEffectSpecWithSetByCallerValue(
		PostureDecreaseSpec,
		GetAbilitySystemComponentFromActorInfo(),
		GE_PostureDecreaseClass,
		GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_PostureDecreaseAmount,
		PostureDecreaseAmount
	);

	if (!bIsPostureRegenSpecValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostureRegenSpec is null in %s, cannot damage apply"), *GetName());
		return;
	}

	GE_PostureDecreaseHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(PostureDecreaseSpec);
}

void UGA_HeroRun::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GE_SpeedBoostHandle.IsValid() && GE_PostureDecreaseHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_SpeedBoostHandle);
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_PostureDecreaseHandle);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("GE_SpeedBoostHandle or GE_PostureDecreaseHandle is null in %s"), *GetName());
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
