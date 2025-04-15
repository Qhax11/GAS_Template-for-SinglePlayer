// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DeathBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "GA_EnemyDeathBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyDeathBase : public UGA_DeathBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void BroadcastDeSpawn() override;
};
