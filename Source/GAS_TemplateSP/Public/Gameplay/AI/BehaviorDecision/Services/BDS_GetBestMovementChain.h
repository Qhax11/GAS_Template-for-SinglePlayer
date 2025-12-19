// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "BDS_GetBestMovementChain.generated.h"

class UMovementDataBase;
class UMovementSingleData;
class UMovementChainData;

USTRUCT(BlueprintType)
struct FAttackAbilityToMovementChain
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> AttackAbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMovementChainsAsset* MovementChainsAsset;
};

UCLASS(BlueprintType)
class UAttackAbilitiesToMovementChainsAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAttackAbilityToMovementChain> AttackAbilityMovementChainMap;
};

UCLASS(BlueprintType)
class UMovementChainsAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UMovementChainData*> MovementChains;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestMovementChain : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UMovementChainData* GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

protected:
    UMovementChainsAsset* GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const;
    /*
    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);
    */

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilitiesToMovementChainsAsset* AttackAbilitiesToMovementChainsAsset;
};
