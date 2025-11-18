// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
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

    FGameplayTag GetDirectionTagFromInput(const FVector2D& Input) const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputDirectionToDodgeMontageAsset* InputDirectionToDodgeMontageAsset;

    class AGAS_HeroBase* HeroBase;
};
