// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_VulnerableBase.h"
#include "GA_EnemyVulnerable.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyVulnerable : public UGA_VulnerableBase
{
	GENERATED_BODY()
	
public:
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
