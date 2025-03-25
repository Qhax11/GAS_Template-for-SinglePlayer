// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_BehaviorDecision.generated.h"


USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UGA_MeleeAttackBase> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownPenalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CounterPenalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoreBias;
};

UCLASS(BlueprintType)
class UAttackAbilityDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackData> AttackAbilities;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

protected:
	UAC_BehaviorDecision();

	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityDataAsset* AttackAbilityData;

    class AAIControllerBase* OwnerController;

public:
    UFUNCTION(BlueprintCallable)
    FAttackData GetBestAttack(float DistanceToTarget);

    UFUNCTION(BlueprintCallable)
    void SendSelectedAttackData();

private:

    FAttackData SelectedAttackAbilityData;
};
