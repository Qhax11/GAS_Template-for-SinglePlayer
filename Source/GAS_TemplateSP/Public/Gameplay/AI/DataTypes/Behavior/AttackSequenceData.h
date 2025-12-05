// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DataAsset.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackData.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "AttackSequenceData.generated.h"

/**
 * A single step in an attack sequence
 * References existing FAttackData instead of duplicating attack info
 */
USTRUCT(BlueprintType)
struct FSequenceStep
{
    GENERATED_BODY()

public:
    // Reference to existing FAttackData (from BDS_GetBestAttack service data)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Step")
    FAttackData AttackToExecute;

    // Wait time after this attack completes (creates tempo/rhythm)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Step")
    float WaitTimeMin = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Step")
    float WaitTimeMax = 0.8f;

    // Can this step be interrupted by incoming attack?
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Step")
    bool bCanBeInterrupted = true;

    // Optional: Force a specific movement after this attack
    // Leave null to use default movement chain selection
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Step")
    UMovementChainAsset* OverrideMovementChain = nullptr;
};

/**
 * UAttackSequenceAsset
 *
 * Defines a pattern/combo of attacks that the AI will execute in order.
 * This is the "intent" that makes AI feel deliberate and pattern-based.
 *
 * Example Sequences:
 * - "AggressiveCombo": Dash → Attack1 → Attack2 → Attack3
 * - "BaitPattern": CircleStrafe → Wait → ParryStance → QuickAttack
 * - "HitAndRun": Dash → Attack → BackstepDodge → Wait
 */
UCLASS(BlueprintType)
class UAttackSequenceAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metadata")
    FName SequenceName;

    // The ordered list of attacks to execute
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
    TArray<FSequenceStep> Steps;

    // Movement chain to use for initial approach
    // This references your existing UMovementChainAsset system
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
    UMovementChainAsset* ApproachMovementChain = nullptr;

    // --- Scoring (used by BehaviorDecision to select which sequence to run) ---

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    float BasePriority = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    UCurveFloat* DistanceScoreCurve = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    TMap<EBehaviorState, float> BehaviorStateModifiers;

    // Distance constraints for this sequence to be considered
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    float MinDistanceToStart = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    float MaxDistanceToStart = 1000.0f;

    // Optional: Only use this sequence if target is moving/not moving
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    bool bRequireTargetMoving = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring")
    bool bRequireTargetStill = false;
};