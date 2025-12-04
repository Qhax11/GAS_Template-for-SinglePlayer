// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackData.h"
#include "BDS_GetBestAttack.generated.h"


UCLASS(BlueprintType)
class UAttackAbilityAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackData> AttackAbilities;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestAttack : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    FAttackData GetBestAttack();

protected:
    float CalculateAttackAbilityScoreBasedOnTargetDistance(FAttackData AttackData, float DistanceToTarget);

    float CalculateComboScore(FAttackData AttackData);

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityAsset* AttackAbilityAsset;

    FAttackData LastSelectedAttackAbilityData;
};
