// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "BDS_GetBestAttack.generated.h"


/**
 * Data asset that owns and configures attack option data used by attack selection services.
 *
 * Responsibility:
 * - Holds editor-configured attack data objects (typically instanced subobjects).
 * - Acts purely as a content/config container; contains no runtime logic.
 * - The service (UBDS_GetBestAttack) is responsible for evaluating and selecting from this data.
 *
 * Notes:
 * - Attack data objects should be treated as immutable configuration.
 * - Do NOT store runtime state inside this asset or its subobjects.
 */
UCLASS(BlueprintType)
class UAttackAbilityAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UAttackDataBase*> OptionalAttacks;
};

/**
 * Behavior decision service responsible for selecting the most appropriate attack
 * based on the current decision context and configured attack data.
 *
 * Responsibilities:
 * - Evaluates attack data provided by UAttackAbilityAsset.
 * - Selects and returns the best matching attack data.
 *
 * Non-responsibilities:
 * - Does NOT create, own, or modify attack data content.
 * - Does NOT store persistent or shared runtime state.
 */
UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestAttack : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UAttackDataBase* GetBestAttack();

protected:
    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityAsset* AttackAbilityAsset;

    UAttackDataBase* LastSelectedAttackAbilityData;
};
