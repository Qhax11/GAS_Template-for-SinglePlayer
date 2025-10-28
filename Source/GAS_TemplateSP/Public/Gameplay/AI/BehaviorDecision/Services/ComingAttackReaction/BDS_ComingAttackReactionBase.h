// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "BDS_ComingAttackReactionBase.generated.h"

UENUM(BlueprintType)
enum class EComingAttackReaction : uint8
{
    TakeDamage  UMETA(DisplayName = "TakeDamage"),
    Parry  UMETA(DisplayName = "Parry"),
    Dodge     UMETA(DisplayName = "Dodge"),
};

UCLASS(BlueprintType)
class UComingAttackReactionAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TObjectPtr<UBDS_ComingAttackReactionBase>> ComingAttackReactions;
};

UCLASS(Blueprintable, DefaultToInstanced, EditInLineNew)
class GAS_TEMPLATESP_API UBDS_ComingAttackReactionBase : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    virtual float CalculateComingAttackReactionScore(FComingAttackPayload ComingAttackPayload);
	
    virtual bool IsEnable(FComingAttackPayload ComingAttackPayload) const;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this Coming Attack Reaction. Used for debugging or referencing in logic."))
    FName ComingAttackReactionName;

    // Defense reaction type this data represents (e.g., Parry or Dodge)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EComingAttackReaction ReactionType;

    // Minimum time required before impact to allow this reaction (otherwise it's too late)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    float MinimumTimeBeforeHitToReact = 0.1f;

    // The time before hit at which this reaction should be triggered
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    float PreferredTriggerTimeBeforeHit = 0.2f;

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

protected:
    float CalculateBehaviorStateScore() const;

    float CalculateTagScore(const FComingAttackPayload ComingAttackPayload) const;

    bool PassesFinalChanceRoll() const;

    bool PassesChanceRoll() const;

    bool PassesChanceRollBasedOnPosture() const;


    UPROPERTY()
    UComingAttackReactionAsset* ComingAttackReactionAsset;
};
