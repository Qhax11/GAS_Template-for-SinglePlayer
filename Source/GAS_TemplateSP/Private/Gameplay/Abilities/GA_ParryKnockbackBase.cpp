// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ParryKnockbackBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
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

	const UGA_MeleeAttackBase* MeleeAttack = Cast<UGA_MeleeAttackBase>(TriggerEventData->ContextHandle.GetAbility());
	if (!MeleeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackBase is null in: %s"), *GetName());
		return;
	}

	FGameplayTag AttackType = GetAttackTypeTagFromMeleeAttack(MeleeAttack);

	MotionWarpingForce = KnockbackDataAsset->FindKnockbackForce(AttackType);
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
}

FGameplayTag UGA_ParryKnockbackBase::GetAttackTypeTagFromMeleeAttack(const UGA_MeleeAttackBase* MeleeAttack)
{
	FGameplayTag AttackDirectionTag;
	for (const FGameplayTag& Tag : MeleeAttack->AbilityTags)
	{
		if (Tag.MatchesTag(GAS_Tags::TAG_Gameplay_Ability_Attack_Type))
		{
			AttackDirectionTag = Tag;
			break;
		}
	}

	return AttackDirectionTag;
}
