// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRouterDashAndRun.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRouterDashAndRun : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> GA_DashAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> GA_RunAbilityClass;

	UFUNCTION()
	void OnDashAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UGAS_GameplayAbilityBase* UsedDashAbilty;

	UGAS_GameplayAbilityBase* UsedRunAbilty;

	class UAbilityTask_WaitInputRelease* WaitRelease;

};
