// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include <AbilitySystemGlobals.h>

UGA_MeleeAttackBase::UGA_MeleeAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Attack);
	SetAssetTags(AssetTags);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
}

void UGA_MeleeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttackBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (bHasEnded)
	{
		return;
	}

	Super::OnEventReceived(EventTag, EventData);

	UE_LOG(LogTemp, Warning, TEXT("%s's %s ability OnEventReceived"), *GetAvatarActorFromActorInfo()->GetName(), *GetName());

	if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_Attack_TraceStart)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_TraceTick, this, &UGA_MeleeAttackBase::TraceTick, TraceTickValue, true, 0);
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_Attack_TraceEnd) 
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
	}

	if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_LockRotationTowardsTarget)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_UnLockRotationTowardsTarget)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget, 100);
	}
}

void UGA_MeleeAttackBase::TraceTick()
{
	TArray<FHitResult> OutHitResults;
	if (TraceForHostileUnits(OutHitResults))
	{
		AttackLogic(OutHitResults);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
	}
}

bool UGA_MeleeAttackBase::TraceForHostileUnits(TArray<FHitResult>& OutHitResults)
{
	if (TraceData)
	{
		if (CharacterBase->GetWeapon())
		{
			FTraceRequest TraceRequest;
			TraceRequest.StartLocation = CharacterBase->GetWeapon()->GetTraceMid();
			TraceRequest.EndLocation = CharacterBase->GetWeapon()->GetTraceEnd();
			TraceRequest.Direction = CharacterBase->GetWeapon()->GetActorRotation();
			TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, OutHitResults, TraceRequest);
		}
	}
	return OutHitResults.IsValidIndex(0);
}

void UGA_MeleeAttackBase::AttackLogic(TArray<FHitResult>& OutHitResults)
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

void UGA_MeleeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

