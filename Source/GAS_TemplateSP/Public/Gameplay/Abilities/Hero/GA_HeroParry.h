// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_ParryBase.h"
#include "GA_HeroParry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroParry : public UGA_ParryBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	virtual void OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
