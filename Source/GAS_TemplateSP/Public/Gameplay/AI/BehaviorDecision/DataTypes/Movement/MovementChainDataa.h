// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "MovementChainDataa.generated.h"

class UGA_EnemyMovementBase;
class UAbilitySystemComponent;
class UMovementSingleData;

/**
 * Represents a scored sequence of movement abilities used by the AI to reposition itself
 * relative to a selected attack. A movement chain is chosen via decision logic (score-based),
 * then executed step-by-step by the MovementManager until completion or interruption.
 *
 * This data does NOT guarantee executability at runtime; it only expresses intent and preference.
 * Actual execution validity (distance, blocking states, interruptions) is handled by states/abilities.
 */
UCLASS()
class GAS_TEMPLATESP_API UMovementChainDataa : public UMovementDataBase
{
	GENERATED_BODY()

public:
    virtual bool IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug = nullptr) const override; 

    virtual bool PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug = nullptr) const override; 

	virtual float GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug = nullptr) const override;

	virtual bool IsChain() const override { return true; }

protected:
	virtual float GetBehaviorStateScore(const FMovementDecisionContext& Context) const;

	virtual float GetDistanceScore(const FMovementDecisionContext& Context) const;

	virtual float GetTargetMovementScore(const FMovementDecisionContext& Context) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this movement chain. Used for debugging or referencing in logic."))
	FName MovementChainName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Sequence of movement abilities that make up this chain. Executed in order."))
	TArray<UMovementSingleData*> MovementChain;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers based on current behavior state (e.g., aggressive, defensive)."))
	TMap<EBehaviorState, float> BehaviorStateModifiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score curve based on distance to target. High values make this chain more likely when far/close depending on the curve."))
	UCurveFloat* DistanceScoreCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score bonus applied if the target is currently moving."))
	float ScoreModifierWhenTargetIsMoving = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score bonus applied if the target is not moving)."))
	float ScoreModifierWhenTargetIsNotMoving = 0.0f;

	// Preferred minimum distance for this movement chain to be considered favorable.
    // Used only in decision scoring, not as an execution constraint.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ToolTip = "Used only in decision scoring, not as an execution constraint."))
	float PreferredMinDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Flat score bias added to this chain's total score. Useful to prioritize certain chains."))
	float ScoreBias = 0.f;
};
