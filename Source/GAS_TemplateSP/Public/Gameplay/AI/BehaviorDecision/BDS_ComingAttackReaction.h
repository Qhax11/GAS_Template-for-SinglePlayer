// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/BehaviorDecisionServiceBase.h"
#include "BDS_ComingAttackReaction.generated.h"

UENUM(BlueprintType)
enum class EComingAttackReaction : uint8
{
    TakeDamage  UMETA(DisplayName = "TakeDamage"),
    Parry  UMETA(DisplayName = "Parry"),
    Dodge     UMETA(DisplayName = "Dodge"),
};

USTRUCT(BlueprintType)
struct FComingAttackReactionData
{
    GENERATED_BODY()

public:
    // Defense reaction type this data represents (e.g., Parry or Dodge)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EComingAttackReaction ReactionType;

    // Base chance to select this reaction (0.0 to 1.0)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BaseChance = 0.5f;

    // Optional modifiers based on behavior state
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    // Flat score bias to encourage/discourage selection
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ScoreBias = 0.f;
};

UCLASS(BlueprintType)
class UComingAttackReactionAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FComingAttackReactionData> ComingAttackReactions;
};


UCLASS()
class GAS_TEMPLATESP_API UBDS_ComingAttackReaction : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    EComingAttackReaction GetComingAttackDecision(struct FComingAttackPayload ComingAttackPayload);
	
    void SetComingAttackReactionAsset(UComingAttackReactionAsset* ComingAttackReactionAsset);

    UComingAttackReactionAsset* ComingAttackReaction;
};
