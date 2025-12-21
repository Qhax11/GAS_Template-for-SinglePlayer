// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/DecisionOptionData.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h" // forward declaration doesen't work, so i put include?? 
#include "AttackDataBase.generated.h"

class UAbilitySystemComponent;

USTRUCT()
struct FAttackDecisionContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Owner = nullptr;

	UPROPERTY()
	AActor* Target = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* OwnerASC = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	EBehaviorState BehaviorState = EBehaviorState::None;
};

UENUM()
enum class EAttackDisableReason : uint8
{
	None,
	InvalidAbility,
	InvalidContext,
	OnCooldown,
	InvalidState,
	ComboBlocked,
};

struct FAttackEnableDebug
{
	EAttackDisableReason Reason = EAttackDisableReason::None;
};

struct FAttackChanceDebug
{
	float Roll = 0.f;
	float Threshold = 0.f;
};

struct FAttackScoreDebug
{
	float BehaviorScore = 0.f;
	float ComboScore = 0.f;
	float DistanceScore = 0.f;
	float Bias = 0.f;
	float Total = 0.f;
};

/**
 * UAttackDataBase
 *
 * Domain-specific decision data representing a **single attack option**
 * that the AI can select during behavior evaluation.
 *
 * This class specializes the generic decision-option pipeline defined in
 * UDecisionOptionData for the **attack-selection domain**.
 *
 * Responsibilities of this class:
 * - Describing which gameplay ability represents this attack
 * - Evaluating whether the attack is currently allowed (IsEnable)
 * - Optionally applying probabilistic commitment rules (PassesChance)
 * - Producing a deterministic score used to rank this attack against others
 *
 * This class does NOT:
 * - Execute the attack ability
 * - Access world state directly (distance, target, combo state are provided via context)
 * - Manage combo chaining or cooldown timing on its own
 *
 * Key concepts:
 * - Static configuration lives in this data object (biases, combo flags, modifiers)
 * - Runtime state is provided via FAttackDecisionContext by the decision service
 * - Final selection is always driven by score, never by chance alone
 *
 * Conceptual role:
 *   "Given the current combat context, how desirable is it for the AI to use this attack?"
 *
 * Concrete subclasses (e.g. combo attacks, special attacks) are expected to override
 * parts of the decision logic to express more specific behavior.
 */

UCLASS(Abstract, Blueprintable, EditInLineNew, DefaultToInstanced)
class GAS_TEMPLATESP_API UAttackDataBase : public UDecisionOptionData
{
	GENERATED_BODY()
	
public:
	virtual bool IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug = nullptr) const;

	virtual bool PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug = nullptr) const;

	virtual float GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug = nullptr) const;

protected:
	float GetDistanceScore(const FAttackDecisionContext& Context) const;

	//float CalculateScoreBasedOnTargetDistance(FAttackData AttackData, float DistanceToTarget);

	//float CalculateComboScore(FAttackData AttackData);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this Attack. Used for debugging or referencing in logic."))
	FName AttackName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Ability class that defines the actual gameplay logic and range values"))
    TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "AI.Cooldown", ToolTip = "You have to choose spesific CooldownTag"))
	FGameplayTag AbilityCooldownTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers per behavior state"))
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Whether this attack is part of a combo chain"))
    bool bIsComboAttack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Combo index used for ordering within a combo chain "), meta = (EditCondition = "bIsComboAttack"))
    int32 ComboIndex = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Base score bias applied to AI decision-making"))
    float ScoreBias = 0.f;
};
