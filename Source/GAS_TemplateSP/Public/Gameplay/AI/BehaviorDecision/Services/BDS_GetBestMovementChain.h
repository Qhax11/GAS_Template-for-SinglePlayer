// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "BDS_GetBestMovementChain.generated.h"

class UMovementDataBase;
class UMovementSingleData;
class UMovementChainData;
class UAttackDataBase;


USTRUCT(BlueprintType)
struct FAttackMovementProfileToChains
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "AI.Decision.Attack.Movement"))
    FGameplayTag MovementAttackTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UMovementChainAsset*> MovementChains;
};

UCLASS(BlueprintType)
class UAttackAbilitiesToMovementChainsAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAttackMovementProfileToChains> AttackAbilityMovementChainMap;
};

UCLASS(BlueprintType)
class UMovementChainAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UMovementChainData* MovementChain;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestMovementChain : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UMovementChainData* GetBestMovementChain(UAttackDataBase* SelectedAttackData);

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForSelectedAttackAbility(UAttackDataBase* SelectedAttackData) const;
    /*
    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);
    */

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilitiesToMovementChainsAsset* AttackAbilitiesToMovementChainsAsset;
};
