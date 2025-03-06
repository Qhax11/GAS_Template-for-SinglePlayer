// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathBase.h"
#include "GA_EnemyDeathWithFinisher.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyDeathWithFinisher : public UGA_EnemyDeathBase
{
	GENERATED_BODY()

public:
	UGA_EnemyDeathWithFinisher();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
