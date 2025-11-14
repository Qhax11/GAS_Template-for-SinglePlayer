// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRun.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRun : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	class UAbilityTask_WaitInputRelease* WaitRelease;

	UPROPERTY(EditDefaultsOnly)
	float SpeedBoost = 500.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_SpeedBoostClass;

	FActiveGameplayEffectHandle GE_SpeedBoostHandle;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
