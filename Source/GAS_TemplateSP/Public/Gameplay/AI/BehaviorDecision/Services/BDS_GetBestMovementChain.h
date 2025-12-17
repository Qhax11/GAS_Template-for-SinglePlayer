// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "BDS_GetBestMovementChain.generated.h"

class UMovementDataBase;
class UMovementSingleData;
class UMovementChainDataa;

USTRUCT(BlueprintType)
struct FAttackAbilityMovementChains
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> AttackAbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UMovementSingleData*> MovementAbility;
};

UCLASS(BlueprintType)
class UAttackAbilityMovementChainMapAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAttackAbilityMovementChains> AttackAbilityMovementChainMap;
};

UCLASS(BlueprintType)
class UMovementChainAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName MovementChainName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowedClasses = "MovementSingleData"))
    TArray<UMovementDataBase*> MovementAbilitiesData;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestMovementChain : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UMovementDataBase* GetBestMovement(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const;
    /*
    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);
    */
    FGameplayTag GetRandomDirectionTag();

    UPROPERTY(EditDefaultsOnly)
    UMovementChainAsset* MovementChainAsset;

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityMovementChainMapAsset* AttackAbilityMovementChainMapAsset;
};
