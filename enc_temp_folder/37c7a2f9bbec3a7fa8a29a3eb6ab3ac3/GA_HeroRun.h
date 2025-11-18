// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRun.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRun : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_HeroRun(); 

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_SpeedBoostClass;

	FActiveGameplayEffectHandle GE_SpeedBoostHandle;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
