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

	UFUNCTION()
	void OnDashAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	void TryActivateRun();

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> GA_DashAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> GA_RunAbilityClass;

	UGAS_GameplayAbilityBase* UsedDashAbilty;

	UGAS_GameplayAbilityBase* UsedRunAbilty;

	class UAbilityTask_WaitInputRelease* WaitRelease;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float MinInputHoldTimeForRun = 0.2f; // 0.2 saniye minimum basýlý tutma süresi

private:
	float InputPressedTime = 0.0f;
};
