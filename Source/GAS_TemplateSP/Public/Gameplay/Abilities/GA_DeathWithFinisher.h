// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/GA_EnemyDeath.h"
#include "GA_DeathWithFinisher.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_DeathWithFinisher : public UGA_EnemyDeath
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
