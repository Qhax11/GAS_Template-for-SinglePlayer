// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackSequenceData.h"
#include "BDS_GetBestSequence.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestSequence : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& InitParams) override;

    UFUNCTION(BlueprintCallable, Category = "Behavior Decision")
    UAttackSequenceAsset* GetBestSequence();

protected:
    // All available sequences for this enemy
    UPROPERTY(EditDefaultsOnly, Category = "Sequences")
    TArray<UAttackSequenceAsset*> AvailableSequences;

    // Score and select best sequence
    UAttackSequenceAsset* ScoreAndSelectBestSequence() const;

    float CalculateSequenceScore(UAttackSequenceAsset* Sequence) const;
    bool IsSequenceValid(UAttackSequenceAsset* Sequence) const;
	
};
