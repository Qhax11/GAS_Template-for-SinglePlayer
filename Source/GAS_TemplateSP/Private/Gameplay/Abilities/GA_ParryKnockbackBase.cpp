// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ParryKnockbackBase.h"
#include "AbilitySystemGlobals.h"

UGA_ParryKnockbackBase::UGA_ParryKnockbackBase()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Knocback);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;

	AbilityTriggers.Add(TriggerData);
}

void UGA_ParryKnockbackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* OwnerInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->Instigator)
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TriggerEventData->Instigator);
	if (!InstigatorASC)
	{
		return;
	}

	FGameplayEffectSpecHandle EffectSpecHandle = InstigatorASC->MakeOutgoingSpec(ParryKnockbackEffect, 1.0f, TriggerEventData->ContextHandle);
	if (!EffectSpecHandle.IsValid())
	{
		return;
	}

	InstigatorASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
