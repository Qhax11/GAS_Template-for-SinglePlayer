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
class UFinisherAttackDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMeleeFinisherAttackPair> FinisherAttackPairs;
};

UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeFinisher : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

public:
	UGA_MeleeFinisher();

    // Data asset referansý
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UFinisherAttackDataAsset* FinisherAttackData;

    // Hero montajý seçme ve oynatma fonksiyonu
    UFUNCTION(BlueprintCallable)
    UAnimMontage* GetRandomHeroMontage(FGameplayTag& OutChosenTag);
};
