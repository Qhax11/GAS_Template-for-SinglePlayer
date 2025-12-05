// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "AttackData.generated.h"

/**
 * FAttackData
 *
 * Metadata used by the AI behavior-selection system to evaluate and choose attack abilities.
 * This struct does not represent runtime combat events. It only defines scoring parameters
 * and configuration values used during AI decision making.
 *
 * Key points:
 * - Contains the ability class associated with the attack.
 * - Stores optional score modifiers per behavior state (e.g., Aggressive, Defensive).
 * - Supports combo-chain ordering through ComboIndex.
 * - Provides a bias value used during weighted attack selection.
 *
 * This type belongs under Behavior data definitions because it is strictly related to
 * decision making and ability evaluation logic, not general combat gameplay.
 */

class UGAS_GameplayAbilityBase;

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

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