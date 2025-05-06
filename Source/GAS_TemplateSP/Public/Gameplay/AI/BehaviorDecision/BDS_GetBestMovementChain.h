// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/BehaviorDecisionServiceBase.h"
#include "BDS_GetBestMovementChain.generated.h"

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

    // How long the ability should stay active before ending automatically (e.g., for chase abilities).
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "How long (in seconds) this movement ability should stay active before ending. Useful for chase or flee behaviors. Set to 0 to ignore."))
    float TimeLimit = 0.f;
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

UCLASS(BlueprintType)
class UAttackAbilityMovementChainMapAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackAbilityMovementChains> ChainMappings;
};

UCLASS()
class GAS_TEMPLATESP_API UBDS_GetBestMovementChain : public UBehaviorDecisionServiceBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams) override;

    TArray<FMovementAbilityData> GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const;

    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);

    FGameplayTag GetRandomDirectionTag();

    UAttackAbilityMovementChainMapAsset* AttackAbilityMovementChainMapAsset;
};
