// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_HeroAirKick.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroAirKick : public UGA_MontageAbility
{
	GENERATED_BODY()
	
public:
	UGA_HeroAirKick();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StunEffect;
};
