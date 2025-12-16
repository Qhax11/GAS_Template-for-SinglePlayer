// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "ComingAttackReactionData.generated.h"

UENUM(BlueprintType)
enum class EComingAttackReaction : uint8
{
    TakeDamage  UMETA(DisplayName = "TakeDamage"),
    Parry  UMETA(DisplayName = "Parry"),
    Dodge     UMETA(DisplayName = "Dodge"),
};

UENUM()
enum class EReactionDisableReason : uint8
{
    None,
    TooLate,
    InvalidDistance,
    PostureTooHigh,
    AttackTagBlocked,
    UnparryableAttack,
    UndodgeableAttack,
    Unstoppable,
    CanInterruptUnstoppable,
    InActiveAttackPhase,
    Cooldown,
};

struct FReactionEnableDebug
{
    bool bEnabled = true;
    EReactionDisableReason Reason = EReactionDisableReason::None;
};

UENUM()
enum class EReactionChanceFailReason : uint8
{
    None,
    RandomRollFailed,
    PostureRollFailed,
};

struct FReactionChanceDebug
{
    EReactionChanceFailReason Reason = EReactionChanceFailReason::None;
    float Roll = 0.f;
    float Threshold = 0.f;
};

struct FReactionScoreDebug
{
    float BehaviorStateScore = 0.f;
    float TagScore = 0.f;
    float Bias = 0.f;
    float Total = 0.f;
};

/**
 * UComingAttackReactionData
 *
 * Represents a single defensive reaction option (e.g. Parry, Dodge, TakeDamage)
 * that can be evaluated by the AI when responding to an incoming attack.
 *
 * Reactions are evaluated in THREE sequential steps:
 *
 * 1) Enable (IsEnable)
 *    - Hard, deterministic gating.
 *    - Answers: "Is this reaction even allowed right now?"
 *    - Examples:
 *        - Too late to react
 *        - Attack is unparryable / undodgeable
 *        - Defender is unstoppable
 *        - Cooldown or state restrictions
 *
 * 2) Chance (PassesChanceRoll)
 *    - Probabilistic eligibility check.
 *    - Answers: "Even if allowed, does the AI commit to this reaction right now?"
 *    - Used to introduce controlled uncertainty and avoid perfectly deterministic behavior.
 *
 * 3) Score (GetScore)
 *    - Deterministic prioritization among the remaining valid reactions.
 *    - Answers: "Which of the remaining reactions is the best choice?"
 *    - Influenced by:
 *        - Current behavior state
 *        - Incoming attack tags
 *        - Flat score bias
 *        - (Later) combat memory and context modifiers
 *
 * IMPORTANT:
 * - Chance does NOT compare reactions against each other.
 * - It only filters out reactions before scoring.
 * - The final selection is ALWAYS driven by score.
 *
 * Conceptual model:
 *   Enable -> Chance -> Score -> Selection
 *
 * This separation allows the AI to:
 * - Prefer aggressive reactions like Parry
 * - Fall back to safer options like Dodge
 * - Occasionally fail or hesitate, creating believable behavior
 *
 * This class is designed to be:
 * - Data-driven (tunable via DataAssets)
 * - Extensible (reaction-specific overrides)
 * - Debuggable (explicit enable, chance, and score breakdowns)
 */


UCLASS(Blueprintable, DefaultToInstanced, EditInLineNew, Abstract)
class UComingAttackReactionData : public UObject
{
    GENERATED_BODY()

public:
    virtual bool IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug = nullptr) const;

    virtual bool PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug = nullptr) const;

    virtual float GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug = nullptr) const;

protected:
    virtual float CalculateBehaviorStateScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState) const;

    virtual float CalculateTagScore(const FComingAttackPayload& ComingAttackPayload) const;

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

    // Base success probability used by PassesChanceRoll() when this reaction is selected.
    // This value represents how reliable the reaction is by default, before posture,
    // combat context, or memory-based modifiers are applied.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BaseChance = 0.5f;

    // Minimum allowed success probability for this reaction.
    // Acts as a safety floor during PassesChanceRoll() so that penalties
    // (low posture, memory debuffs, bad context) never make the reaction impossible.
    // This prevents the AI from feeling "broken" or completely helpless.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MinChance = 0.1f;

    // Maximum allowed success probability for this reaction.
    // Acts as a safety ceiling during PassesChanceRoll() so that bonuses
    // (high posture, favorable context, memory buffs) never make the reaction guaranteed.
    // This preserves uncertainty and prevents deterministic AI behavior.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MaxChance = 0.95f;

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