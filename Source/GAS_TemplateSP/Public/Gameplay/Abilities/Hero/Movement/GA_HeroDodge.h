// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DodgeBase.h"
#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GA_HeroDodge.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDodge : public UGA_DodgeBase
{
	GENERATED_BODY()

public:
    UGA_HeroDodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

    UFUNCTION()
    void OnAfterFrame();

    FGameplayTag GetDirectionTagFromInput(const FVector2D& Input) const;

    virtual FVector CalculateMotionWarpingLocation() const override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnPerfectDodgeReceivedBP();

    UFUNCTION()
    void RemoveDamageImmuneTag();

    virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

    void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

    UPROPERTY()
    class UAT_WaitOneFrame* WaitOneFrameTask;

    UPROPERTY()
    class UAbilityTask_WaitDelay* WaitDelayTask;

    UPROPERTY()
    class AGAS_HeroBase* HeroBase;

    UPROPERTY()
    class UAC_HeroControl* HeroControlComponent;
};
