// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "GA_MeleeFinisher.generated.h"

USTRUCT(BlueprintType)
struct FMeleeFinisherAttackPair
{
    GENERATED_BODY()

    // Tag ile varyasyonu belirle
    UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (Categories = "Gameplay.Ability.Combat.Attack.MeleeFinisher"))
    FGameplayTag FinisherTag;

    // Hero finisher montajý
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UAnimMontage* FinisherAttackMontage;
};

UCLASS(BlueprintType)
class UDA_FinisherAttackMontage: public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMeleeFinisherAttackPair> FinisherAttackPairs;

    // Returns the montage associated with the given FinisherTag
    UFUNCTION(BlueprintCallable, Category = "Finisher")
    UAnimMontage* FindMontageByTag(FGameplayTag FinisherTag) const
    {
        for (const FMeleeFinisherAttackPair& Pair : FinisherAttackPairs)
        {
            if (Pair.FinisherTag == FinisherTag)
            {
                return Pair.FinisherAttackMontage;
            }
        }
        return nullptr;
    }
};

UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeFinisher : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

public:
	UGA_MeleeFinisher();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

    // Data asset referansý
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UDA_FinisherAttackMontage* DA_FinisherAttackMontage;

};
