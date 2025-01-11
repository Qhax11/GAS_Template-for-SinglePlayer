// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include <AbilitySystemGlobals.h>

void UGA_MeleeAttackBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	TArray<FHitResult> OutHitResults;
	TraceForHostileUnits(OutHitResults);

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

void UGA_MeleeAttackBase::TraceForHostileUnits(TArray<FHitResult>& OutHitResults)
{
	if (TraceData)
	{
		if (AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()))
		{
			FVector TraceLocation = CharacterBase->GetWeapon()->GetComponentLocation();
			FRotator TraceRotation = CharacterBase->GetWeapon()->GetComponentRotation();
			TraceData->Trace->CreateTraceWithTeamFilterWithLocationAndDirection(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, TraceLocation, TraceRotation, OutHitResults);

			if(OutHitResults.IsValidIndex(0))
			UE_LOG(LogTemp, Error, TEXT("Location: %s"), *OutHitResults[0].Location.ToString());
		}
		//TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, OutActors);
	}
}