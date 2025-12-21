// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "MovementSingleData.generated.h"

class UAbilitySystemComponent;

/**
 * UMovementSingleData
 *
 * Decision-time data describing a **single movement action** that the AI can execute
 * as part of a movement chain or as a standalone movement option.
 *
 * This class is purely **data-driven** and contains no execution logic.
 * It defines *what* movement ability can be used and *how* it should be parameterized
 * when activated by the MovementManager.
 *
 * Responsibilities:
 * - Reference the gameplay ability that performs the movement
 * - Provide the gameplay event tag used to trigger the ability
 * - Define optional direction-resolution rules (policy-based)
 * - Store runtime-resolved direction data for the current execution
 * - Provide optional magnitude data (distance, duration, etc.)
 *
 * Important design notes:
 * - DirectionPolicyTag and ResolvedDirectionTag are **decision/execution glue**:
 *   the policy is configured in data, the resolved tag is written at runtime right before ability activation.
 * - This data object does NOT:
 * - Check distance or validity
 * - Decide when the movement should be used
 * - Execute or activate the ability
 *
 * Execution flow (high level):
 *   MovementDecision ? MovementChain selected ?
 *   MovementManager resolves direction ?
 *   GameplayAbility activated with event data
 *
 * Conceptually:
 *   "If the AI decides to move this way, which ability should it run and with what parameters?"
 */
UCLASS(EditInlineNew, DefaultToInstanced)
class GAS_TEMPLATESP_API UMovementSingleData : public UMovementDataBase
{
	GENERATED_BODY()

public:
    bool IsValidData() const;

public:
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Name of this Movement. Used for debugging or referencing in logic."))
    FName MovementName;

    // Activated by the MovementManager using a gameplay event.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Gameplay Ability class that performs the actual movement logic."))
    TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

    // The gameplay tag used to trigger this ability.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.AbilityTriggerEvent.Movement", ToolTip = "Gameplay event tag used to trigger the movement ability."))
    FGameplayTag AbilityTriggerTag;

    // Defines how close the AI must get to consider the movement completed.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Acceptance radius used by MoveTo-style movement abilities"))
    float AcceptanceRadius = 20.0f;

    // Interpreted by the movement ability or task.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Movement speed applied while this movement ability is active."))
    float MovementSpeed = 200.0f;

    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Expected duration of this movement, used for planning and flow control."))
    float ExpectedDuration = 2.0f;

    // Used to prevent extremely short or jittery movements.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Minimum time the movement should remain active before it is allowed to finish."))
    float MinDuration = 0.25f;

    // Prevents stuck or runaway movement executions.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = " Maximum allowed duration for this movement as a safety limit."))
    float MaxDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gameplay.Direction", ToolTip = "Direction tag to be passed directly to the movement ability."))
    FGameplayTag DirectionTag;

    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Enables runtime direction resolution via a policy instead of a fixed direction."))
    bool EnableDirectionPolicy = false;

    // The policy used to resolve the direction, like random or based on player position.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Policy", EditCondition = "EnableDirectionPolicy", ToolTip = "Defines how the direction should be resolved at runtime (e.g., LastPlayerDirection, Random)."))
    FGameplayTag DirectionPolicyTag;
};
