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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this Coming Attack Reaction. Used for debugging or referencing in logic."))
    FName ComingAttackReactionName;

    // Defense reaction type this data represents (e.g., Parry or Dodge)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EComingAttackReaction ReactionType;

    // Base chance to select this reaction (0.0 to 1.0)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BaseChance = 0.5f;

    // +X score if AI is in this state
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    // +X score if the incoming attack has these tags
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<FGameplayTag, float> TagScoreModifiers;

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
	
protected:
    float CalculateBehaviorStateScore(const FComingAttackReactionData& Data) const;

    float CalculateTagScore(const FComingAttackReactionData& Data, const FComingAttackPayload ComingAttackPayload) const;

    bool PassesChanceRoll(const FComingAttackReactionData& ReactionData) const;

    UComingAttackReactionAsset* ComingAttackReactionAsset;
};
