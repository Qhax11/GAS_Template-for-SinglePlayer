// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include <AbilitySystemGlobals.h>

UGA_MeleeAttackBase::UGA_MeleeAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack);
	SetAssetTags(AssetTags);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
}

void UGA_MeleeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	CharacterWeapon = CharacterBase->GetWeapon();
	if (!CharacterWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterWeapon is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}
}

void UGA_MeleeAttackBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);

	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_LockRotationTowardsTarget)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_UnLockRotationTowardsTarget)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget, 100);
	}

	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Weapon_StorePreviousLocation)
	{
		CharacterWeapon->UpdatePreviousLocation();
	}
}

void UGA_MeleeAttackBase::OnTraceHitResults(const TArray<FHitResult>& HitResults)
{
	AttackLogic(HitResults);
}

void UGA_MeleeAttackBase::AttackLogic(const TArray<FHitResult>& OutHitResults)
{
	if (!OutHitResults.IsValidIndex(0))
	{
		return;
	}

	FGameplayEffectSpec DamageSpec;
	bool bIsDamageSpecValid = UGAS_EffectBlueprintFunctionLibary::CreateInstantEffectSpecWithSetByCallerValueWithMoreData(
		DamageSpec,
		GetAbilitySystemComponentFromActorInfo(),
		GEPhysicalDamage,
		GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_DamageAmount,
		Damage.GetValueAtLevel(GetAbilityLevel()),
		OutHitResults[0],
		this
	);

	if (!bIsDamageSpecValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageSpec is null in %s, cannot damage apply"), *GetName());
		return;
	}

	UGAS_EffectBlueprintFunctionLibary::AddTagsToEffectSpecWithContain(DamageSpec, TagsToAddToPhysicalDamageEffect);

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OutHitResults[0].GetActor()))
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(DamageSpec, TargetASC);
	}
}

FGameplayTag UGA_MeleeAttackBase::GetAttackTypeTagFromAbilityTags() const
{
	for (const FGameplayTag& Tag : AbilityTags)
	{
		if (Tag.MatchesTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type))
		{
			return Tag;
		}
	}

	return FGameplayTag(); 
}

FGameplayTag UGA_MeleeAttackBase::GetAttackDirectionTagFromAbilityTags() const
{
	for (const FGameplayTag& Tag : AbilityTags)
	{
		if (Tag.MatchesTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

void UGA_MeleeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

