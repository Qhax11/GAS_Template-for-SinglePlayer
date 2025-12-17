// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "BDS_GetBestAttack.generated.h"


UCLASS(BlueprintType)
class UAttackAbilityAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UAttackDataBase*> OptionalAttacks;
};

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
