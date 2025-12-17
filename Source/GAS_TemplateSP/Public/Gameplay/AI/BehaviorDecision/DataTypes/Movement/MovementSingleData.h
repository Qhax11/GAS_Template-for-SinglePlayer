// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementDataBase.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "MovementSingleData.generated.h"

class UGAS_GameplayAbilityBase;
class UAbilitySystemComponent;

UCLASS(EditInlineNew, DefaultToInstanced)
class GAS_TEMPLATESP_API UMovementSingleData : public UMovementDataBase
{
	GENERATED_BODY()

public:
    // The gameplay ability class used for movement.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

    // The gameplay tag used to trigger this ability.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.AbilityTriggerEvent.Movement"))
    FGameplayTag AbilityTriggerTag;

    UPROPERTY(EditDefaultsOnly)
    bool EnableDirectionPolicy = false;

    // The resolved direction for this ability, typically determined at runtime by a direction policy.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Resolved", EditCondition = "EnableDirectionPolicy", ToolTip = "Resolved direction tag used by this ability at runtime. Typically set based on the direction policy."))
    FGameplayTag ResolvedDirectionTag;

    // The policy used to resolve the direction, like random or based on player position.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Policy", EditCondition = "EnableDirectionPolicy", ToolTip = "Defines how the direction should be resolved at runtime (e.g., LastPlayerDirection, Random)."))
    FGameplayTag DirectionPolicyTag;

    // Used as movement distance for dash abilities, or as a time limit (in seconds) for other movement types like chase or flee.
    // Set to 0 to ignore. For dash, this represents how far the actor should move. For chase/flee, this limits how long the ability stays active.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Used as movement distance for dash abilities, or as a time limit (in seconds) for chase/flee behaviors. Set to 0 to ignore."))
    float AbilityEventMagnitude = 0.f;
};
