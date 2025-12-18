// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/DecisionOptionData.h"
#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
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
    OutOfRange,
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
 * Domain-specific decision data representing a **defensive reaction**
 * to an incoming enemy attack (e.g. Parry, Dodge, TakeDamage).
 *
 * This class specializes the generic AI decision-option model defined in
 * UDecisionOptionData for the **incoming-attack reaction domain**.
 *
 * Responsibilities of this class:
 * - Interpreting incoming-attack context (timing, distance, tags, posture, states)
 * - Defining reaction-specific enable rules (e.g. unparryable / undodgeable attacks)
 * - Defining reaction-specific chance logic (e.g. posture-based commitment)
 * - Providing deterministic scoring biases for reaction prioritization
 *
 * IMPORTANT:
 * - The overall decision pipeline (Enable ? Chance ? Score) is defined in
 *   UDecisionOptionData and intentionally not redefined here.
 * - This class only implements **reaction-specific logic** on top of that pipeline.
 *
 * Conceptual role:
 *   "Given an incoming attack, how should the AI respond defensively?"
 *
 * This separation allows:
 * - A shared decision model across attack, movement, and reaction systems
 * - Clear domain boundaries
 * - Consistent debugging while avoiding duplicated explanations
 */

UCLASS(Blueprintable, DefaultToInstanced, EditInLineNew, Abstract)
class UComingAttackReactionData : public UDecisionOptionData
{
    GENERATED_BODY()

public:
    // Hard gate
    virtual bool IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug = nullptr) const;

    virtual bool PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug = nullptr) const;

    virtual float GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug = nullptr) const;

protected:
    // Checks whether the incoming attack can currently reach the defender.
    // This is a hard, binary validation based on the attack's effective range.
    bool IsAttackInRange(const FComingAttackPayload& ComingAttackPayload) const;

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