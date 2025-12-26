// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroInteraction.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"

void UGA_HeroInteraction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!TraceData || !TraceData->Trace)
	{
		return;
	}

	TArray<AActor*> OutActors;
	TraceData->Trace->CreateTrace(GetWorld(), GetAvatarActorFromActorInfo(), OutActors);
	if (OutActors.IsEmpty()) 
	{
		return;
	}

	BP_OnInteractionSuccess(OutActors[0]);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}


