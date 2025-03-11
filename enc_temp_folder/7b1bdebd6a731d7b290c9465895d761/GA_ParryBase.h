// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_ParryBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_ParryBase : public UGA_MontageAbility
{
	GENERATED_BODY()
	
public:
	UGA_ParryBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);


};
