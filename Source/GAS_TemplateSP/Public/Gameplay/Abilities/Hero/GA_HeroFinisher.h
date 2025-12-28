// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeFinisher.h"
#include "GA_HeroFinisher.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroFinisher : public UGA_MeleeFinisher
{
	GENERATED_BODY()

public:
	UGA_HeroFinisher();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	
};
