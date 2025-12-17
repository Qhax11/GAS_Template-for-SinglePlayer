// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "MovementChainDataa.generated.h"

class UGA_EnemyMovementBase;
class UAbilitySystemComponent;
class UMovementSingleData;

UCLASS()
class GAS_TEMPLATESP_API UMovementChainDataa : public UMovementDataBase
{
	GENERATED_BODY()

public:
    virtual bool IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug = nullptr) const { return true; }

    virtual bool PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug = nullptr) const { return true; }

    virtual float GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug = nullptr) const { return 0.f; }

	virtual bool IsChain() const override { return true; }

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
};
