// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "Gameplay/AI/States/Boss/Boss_State_InComingAttack.h"
#include "BDS_ComingAttackReactionBase.generated.h"

class ComingAttackReactionData;

UCLASS(BlueprintType)
class UComingAttackReactionAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UComingAttackReactionData*> ComingAttackReactions;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_ComingAttackReactionBase : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    UComingAttackReactionData* GetBestComingAttackReaction(FComingAttackPayload& ComingAttackPayload);

protected:
    UPROPERTY(EditDefaultsOnly)
    UComingAttackReactionAsset* ComingAttackReactionAsset;
};
