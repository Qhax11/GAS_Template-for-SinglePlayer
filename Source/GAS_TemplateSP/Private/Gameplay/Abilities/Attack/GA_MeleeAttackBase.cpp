// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include <AbilitySystemGlobals.h>

void UGA_MeleeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttackBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == GAS_Tags::TAG_Gameplay_AttackEvent_TraceStart)
	{
		// Tick is 0.01f
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_TraceTick, this, &UGA_MeleeAttackBase::TraceTick, TraceTickValue, true, 0);
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_AttackEvent_TraceEnd) 
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
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
		if (AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()))
		{
			FVector TraceLocation = CharacterBase->GetWeapon()->GetComponentLocation();
			FRotator TraceRotation = CharacterBase->GetWeapon()->GetComponentRotation();
			TraceData->Trace->CreateTraceWithTeamFilterWithLocationAndDirection(
				GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, TraceLocation, TraceRotation, OutHitResults);
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


