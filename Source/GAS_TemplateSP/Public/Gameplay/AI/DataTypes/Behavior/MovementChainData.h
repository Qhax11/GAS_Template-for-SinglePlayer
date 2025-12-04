// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "MovementChainData.generated.h"

class UGAS_GameplayAbilityBase;
struct FMovementAbilityData;

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
    None         UMETA(DisplayName = "None"),
    Forward      UMETA(DisplayName = "Forward"),
    Backward     UMETA(DisplayName = "Backward"),
    Left         UMETA(DisplayName = "Left"),
    Right        UMETA(DisplayName = "Right")
};

USTRUCT(BlueprintType)
struct FMovementAbilityData
{
    GENERATED_BODY()

public:
    // The gameplay ability class used for movement.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

    // The gameplay tag used to trigger this ability.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.AbilityTriggerEvent.Movement"))
    FGameplayTag AbilityTriggerTag;

    // The resolved direction for this ability, typically determined at runtime by a direction policy.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Resolved", ToolTip = "Resolved direction tag used by this ability at runtime. Typically set based on the direction policy."))
    FGameplayTag ResolvedDirectionTag;

    // The policy used to resolve the direction, like random or based on player position.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Policy", ToolTip = "Defines how the direction should be resolved at runtime (e.g., LastPlayerDirection, Random)."))
    FGameplayTag DirectionPolicyTag;

    // Used as movement distance for dash abilities, or as a time limit (in seconds) for other movement types like chase or flee.
    // Set to 0 to ignore. For dash, this represents how far the actor should move. For chase/flee, this limits how long the ability stays active.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Used as movement distance for dash abilities, or as a time limit (in seconds) for chase/flee behaviors. Set to 0 to ignore."))
    float AbilityEventMagnitude = 0.f;
};

UCLASS(BlueprintType)
class UMovementChainAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this movement chain. Used for debugging or referencing in logic."))
    FName MovementChainName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Sequence of movement abilities that make up this chain. Executed in order."))
    TArray<FMovementAbilityData> MovementChain;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers based on current behavior state (e.g., aggressive, defensive)."))
    TMap<EBehaviorState, float> BehaviorStateModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score curve based on distance to target. High values make this chain more likely when far/close depending on the curve."))
    UCurveFloat* DistanceScoreCurve = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score bonus applied if the target is currently moving."))
    float ScoreModifierWhenTargetIsMoving = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Score bonus applied if the target is not moving)."))
    float ScoreModifierWhenTargetIsNotMoving = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Minimum target distance required for this chain to be considered."))
    float MinRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Flat score bias added to this chain's total score. Useful to prioritize certain chains."))
    float ScoreBias = 0.f;
};

USTRUCT(BlueprintType)
struct FAttackAbilityMovementChains
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> AttackAbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UMovementChainAsset*> MovementChainAssets;
};

