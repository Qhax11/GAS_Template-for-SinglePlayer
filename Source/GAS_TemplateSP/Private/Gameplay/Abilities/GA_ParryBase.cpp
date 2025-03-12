// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ParryBase.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Effects/Calculations/EC_Base.h"

UGA_ParryBase::UGA_ParryBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Parry);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Parry);
}

void UGA_ParryBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ParryBase::Triggered(const FExecCalculationParameters& CalculationParams)
{
	// For some reason GetAbilitySystemComponentFromActorInfo() function returns nullptr, so we use CalculationParams insted.
	UAbilitySystemComponent* OwnerASC = CalculationParams.TargetASC;
	if (!ParryEffect && !OwnerASC)
	{
		return;
	}
	
	FGameplayEffectSpecHandle EffectSpecHandle = OwnerASC->MakeOutgoingSpec(ParryEffect, 1.0f, CalculationParams.GetSpec().GetEffectContext());
	if (!EffectSpecHandle.IsValid())
	{
		return;
	}

	OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

	BP_ApplyForce(CalculationParams.SourceActor, CalculationParams.TargetActor);
}
