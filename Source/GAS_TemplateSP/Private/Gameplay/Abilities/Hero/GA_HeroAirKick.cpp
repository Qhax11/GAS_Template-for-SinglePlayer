// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroAirKick.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

UGA_HeroAirKick::UGA_HeroAirKick()
{
	ActivationRequiredTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_AirKick);
}

void UGA_HeroAirKick::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!OwnerCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	/*
	TArray<FHitResult> HitResults;
	FTraceRequest TraceRequest;
	TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, HitResults, TraceRequest);

	if (!HitResults.IsValidIndex(0)) 
	{
		return;
	}

	AActor* Target = HitResults[0].GetActor();
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);


	UGameplayEffect* GE_Ghost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(StunEffect);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToTarget(GE_Ghost, TargetASC);
	*/
}