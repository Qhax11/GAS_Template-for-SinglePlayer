// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "BDS_GetBestAttack.generated.h"

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Ability class that defines the actual gameplay logic and range values"))
    TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers per behavior state"))
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Whether this attack is part of a combo chain"))
    bool bIsComboAttack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Combo index used for ordering within a combo chain "), meta = (EditCondition = "bIsComboAttack"))
    int32 ComboIndex = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Base score bias applied to AI decision-making"))
    float ScoreBias = 0.f;
};

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
