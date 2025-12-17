// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/DecisionOptionData.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "AttackDataBase.generated.h"

class UGAS_GameplayAbilityBase;
class UAbilitySystemComponent;

USTRUCT()
struct FAttackDecisionContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UAbilitySystemComponent* EnemyASC = nullptr;

	UPROPERTY()
	AActor* Target = nullptr;

	UPROPERTY()
	float TargetDistance = 0.f;

	UPROPERTY()
	EBehaviorState BehaviorState = EBehaviorState::None;

	UPROPERTY()
	bool bIsInCombo = false;
};

UENUM()
enum class EAttackDisableReason : uint8
{
	None,
	InvalidAbility,
	InvalidTarget,
	OutOfRange,
	Cooldown,
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
	float Bias = 0.f;
	float Total = 0.f;
};

UCLASS(Abstract, Blueprintable, EditInLineNew, DefaultToInstanced)
class GAS_TEMPLATESP_API UAttackDataBase : public UDecisionOptionData
{
	GENERATED_BODY()
	
public:
	virtual bool IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug = nullptr) const;

	virtual bool PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug = nullptr) const;

	virtual float GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug = nullptr) const;

protected:

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Ability class that defines the actual gameplay logic and range values"))
    TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers per behavior state"))
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Whether this attack is part of a combo chain"))
    bool bIsComboAttack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Combo index used for ordering within a combo chain "), meta = (EditCondition = "bIsComboAttack"))
    int32 ComboIndex = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Base score bias applied to AI decision-making"))
    float ScoreBias = 0.f;
};
