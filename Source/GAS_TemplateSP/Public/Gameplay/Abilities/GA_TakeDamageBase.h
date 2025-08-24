// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_TakeDamageBase.generated.h"

USTRUCT(BlueprintType)
struct FAttackHitMontagePair
{
    GENERATED_BODY()

    // Attack montage we receive
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> AttackMontage;

    // Reaction montage we want to play
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> HitMontage;
};

UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UAttackToHitMontageAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // List of attack-hit montage mappings
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackHitMontagePair> AttackHitMappings;

    // Helper to lookup hit montage from attack montage
    UAnimMontage* FindHitMontage(UAnimMontage* AttackMontage) const
    {
        for (const FAttackHitMontagePair& Pair : AttackHitMappings)
        {
            if (Pair.AttackMontage.LoadSynchronous() == AttackMontage)
            {
                return Pair.HitMontage.LoadSynchronous();
            }
        }
        return nullptr;
    }
};

UCLASS()
class GAS_TEMPLATESP_API UGA_TakeDamageBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_TakeDamageBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UAnimMontage* GetHitMontage(UAnimMontage* AttackMontage);

    UPROPERTY(EditDefaultsOnly, Category = "TakeDamageBase")
    UAttackToHitMontageAsset* ReactionDataAsset;
};
