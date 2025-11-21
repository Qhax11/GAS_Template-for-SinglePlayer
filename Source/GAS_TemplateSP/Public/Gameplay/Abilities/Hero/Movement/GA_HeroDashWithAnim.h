// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GA_HeroDashWithAnim.generated.h"

USTRUCT(BlueprintType)
struct FInputDirectionDodgeMontagePair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Direction"))
    FGameplayTag InputDirectionTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> DodgeMontage;
};


UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UInputDirectionToDodgeMontageAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FInputDirectionDodgeMontagePair> InputDirectionDodgeMontagePair;

    UAnimMontage* FindDodgetMontage(FGameplayTag InComingAttackDirectionTag) const
    {
        for (const FInputDirectionDodgeMontagePair& Pair : InputDirectionDodgeMontagePair)
        {
            if (Pair.InputDirectionTag == InComingAttackDirectionTag)
            {
                return Pair.DodgeMontage.LoadSynchronous();
            }
        }
        return nullptr;
    }
};

UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDashWithAnim : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
    UGA_HeroDashWithAnim();

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputDirectionToDodgeMontageAsset* InputDirectionToDodgeMontageAsset;

    UPROPERTY()
    class UAT_WaitOneFrame* WaitOneFrameTask;

    UPROPERTY()
    class UAbilityTask_WaitDelay* WaitDelayTask;

    UPROPERTY()
    class AGAS_HeroBase* HeroBase;

    UPROPERTY()
    class UAC_HeroControl* HeroControlComponent;
};
