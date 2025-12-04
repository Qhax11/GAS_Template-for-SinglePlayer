// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "BDS_GetBestMovementChain.generated.h"

struct FMovementAbilityData;


UCLASS(BlueprintType)
class UAttackAbilityMovementChainMapAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackAbilityMovementChains> ChainMappings;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestMovementChain : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UMovementChainAsset* GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const;

    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityMovementChainMapAsset* AttackAbilityMovementChainMapAsset;
};
