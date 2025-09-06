// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathBase.h"
#include "GA_EnemyDeathWithFinisher.generated.h"

USTRUCT(BlueprintType)
struct FMeleeFinisherHitPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.MeleeFinisher"))
    FGameplayTag FinisherTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UAnimMontage* FinisherHitMontage;
};

UCLASS(BlueprintType)
class UFinisherHitDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMeleeFinisherHitPair> FinisherPairs;
};

UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyDeathWithFinisher : public UGA_EnemyDeathBase
{
	GENERATED_BODY()

public:
	UGA_EnemyDeathWithFinisher();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

    // Data asset referansý
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UFinisherHitDataAsset* FinisherHitData;
};
