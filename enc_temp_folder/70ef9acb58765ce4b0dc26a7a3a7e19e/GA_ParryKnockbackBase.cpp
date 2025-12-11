// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/GA_ParryKnockbackBase.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "AbilitySystemGlobals.h"
#include <Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h>

UGA_ParryKnockbackBase::UGA_ParryKnockbackBase()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Knocback);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;

	AbilityTriggers.Add(TriggerData);
}

bool UGA_ParryKnockbackBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// ActorInfo'dan ASC'yi al (InstancedPerExecution için güvenli)
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}

	// Posture check
	if (const UAS_Base* BaseAttributes = ASC->GetSet<UAS_Base>())
	{
		return BaseAttributes->GetPosture() > 0.f;
	}

	return false;
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

	FGameplayEffectSpecHandle ParryKnockbackSpecHandle = InstigatorASC->MakeOutgoingSpec(GE_ParryKnockbackClass, 1.0f, TriggerEventData->ContextHandle);
	if (!ParryKnockbackSpecHandle.IsValid())
	{
		return;
	}

	const UGA_MeleeAttackBase* MeleeAttack = Cast<UGA_MeleeAttackBase>(TriggerEventData->ContextHandle.GetAbility());
	if (!MeleeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackBase is null in: %s"), *GetName());
		return;
	}

	FGameplayTag AttackType = GetAttackTypeTagFromMeleeAttack(MeleeAttack);
	MotionWarpingDistance = KnockbackDataAsset->FindKnockbackForce(AttackType);

	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	// Apply the knockback effect *after* playing the montage.
    // Applying it beforehand may trigger the vulnerable ability,
    // which interrupts or cancels the vulnerable montage.
	InstigatorASC->ApplyGameplayEffectSpecToTarget(*ParryKnockbackSpecHandle.Data, GetAbilitySystemComponentFromActorInfo());

	UGameplayEffect* GE_ParryKnockbackCost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GE_ParryKnockbackCostClass);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(GE_ParryKnockbackCost, 1, FGameplayEffectContextHandle());
}

FGameplayTag UGA_ParryKnockbackBase::GetAttackTypeTagFromMeleeAttack(const UGA_MeleeAttackBase* MeleeAttack)
{
	FGameplayTag AttackDirectionTag;
	for (const FGameplayTag& Tag : MeleeAttack->AbilityTags)
	{
		if (Tag.MatchesTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type))
		{
			AttackDirectionTag = Tag;
			break;
		}
	}

	return AttackDirectionTag;
}
