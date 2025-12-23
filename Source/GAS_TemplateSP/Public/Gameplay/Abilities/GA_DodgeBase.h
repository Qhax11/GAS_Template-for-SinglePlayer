// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_DodgeBase.generated.h"


USTRUCT(BlueprintType)
struct FDodgeMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoitonWarpingDistance = 300;
};

USTRUCT(BlueprintType)
struct FDirectionDodgeMontagePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Direction"))
	FGameplayTag DirectionTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDodgeMontageData MontageData;
};

UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UDirectionToDodgeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDirectionDodgeMontagePair> DirectionDodgeMontagePair;

	UAnimMontage* FindDodgetMontage(FGameplayTag InComingAttackDirectionTag) const
	{
		for (const FDirectionDodgeMontagePair& Pair : DirectionDodgeMontagePair)
		{
			if (Pair.DirectionTag == InComingAttackDirectionTag)
			{
				return Pair.MontageData.DodgeMontage.LoadSynchronous();
			}
		}
		return nullptr;
	}

	float FindMotionWarpingDistance(FGameplayTag InComingAttackDirectionTag) const
	{
		for (const FDirectionDodgeMontagePair& Pair : DirectionDodgeMontagePair)
		{
			if (Pair.DirectionTag == InComingAttackDirectionTag)
			{
				return Pair.MontageData.MoitonWarpingDistance;
			}
		}
		return 200;
	}
};

UCLASS()
class GAS_TEMPLATESP_API UGA_DodgeBase : public UGA_MontageAbility
{
	GENERATED_BODY()
	
protected:
	UGA_DodgeBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual FVector CalculateMotionWarpingLocation() const override;

	UFUNCTION()
	void RemoveDamageImmuneTag();

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY()
	class UAbilityTask_WaitDelay* WaitDelayTask;

	UPROPERTY(EditDefaultsOnly)
	float DodgeDamageImmunityDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDirectionToDodgeDataAsset* DirectionToDodgeDataAsset;
};
