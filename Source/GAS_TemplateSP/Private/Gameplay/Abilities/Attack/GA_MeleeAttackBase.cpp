// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include <AbilitySystemGlobals.h>

void UGA_MeleeAttackBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	TArray<AActor*> OutResultActors;
	TraceForHostileUnits(OutResultActors);

	if (OutResultActors.IsEmpty()) 
	{
		return;
	}

	FGameplayEffectSpec DamageSpec;
	bool bIsDamageSpecValid = UGAS_EffectBlueprintFunctionLibary::CreateInstantEffectSpecWithSetByCallerValue(
		DamageSpec,
		GetAbilitySystemComponentFromActorInfo(),
		GEPhysicalDamage,
		GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_DamageAmount,
		Damage.GetValueAtLevel(GetAbilityLevel()),
		this
		);

	if (!bIsDamageSpecValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageSpec is null in %s, cannot damage apply"), *GetName());
		return;
	}

	UGAS_EffectBlueprintFunctionLibary::AddTagsToEffectSpecWithContain(DamageSpec, TagsToAddToPhysicalDamageEffect);

	for (AActor* CollectedActor : OutResultActors)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CollectedActor))
		{
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(DamageSpec, TargetASC);
		}
	}
}

void UGA_MeleeAttackBase::TraceForHostileUnits(TArray<AActor*>& OutActors)
{
	if (TraceData)
	{
		if (AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()))
		{
			FVector TraceLocation = CharacterBase->GetWeapon()->GetComponentLocation();
			FRotator TraceRotation = CharacterBase->GetWeapon()->GetComponentRotation();
			TraceData->Trace->CreateTraceWithTeamFilterWithLocationAndDirection(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, TraceLocation, TraceRotation, OutActors);
		}
		//TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, OutActors);
	}
}