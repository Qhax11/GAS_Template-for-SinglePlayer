// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "AC_BehaviorDecision.generated.h"

UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
    None        UMETA(DisplayName = "None"),
    Passive     UMETA(DisplayName = "Passive"),
    Aggressive  UMETA(DisplayName = "Aggressive"),
    Defensive   UMETA(DisplayName = "Defensive"),
    Confused    UMETA(DisplayName = "Confused") 
};

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Ability class that defines the actual gameplay logic and range values"))
    TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AbilityCooldownTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Optional score modifiers per behavior state"))
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Minimum effective range. This value is automatically filled from the selected ability's CDO if left as 0."))
    float MinRange = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Maximum effective range. This value is automatically filled from the selected ability's CDO if left as 0."))
    float MaxRange = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Penalty applied when the ability has been recently countered"))
    float CounterPenalty = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Base score bias applied to AI decision-making"))
    float ScoreBias = 0.f;
};

UCLASS(BlueprintType)
class UAttackAbilityAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackData> AttackAbilities;

protected:
    virtual void PostLoad() override
    {
        Super::PostLoad();

        // Automatically sync range values from the associated ability CDOs.
        // This is necessary because systems like StateTree require static data from the asset and cannot fetch values directly from ability CDOs at runtime.
        for (FAttackData& AttackData : AttackAbilities)
        {
            if (AttackData.AbilityClass)
            {
                const UGAS_GameplayAbilityBase* CDO = AttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
                AttackData.MaxRange = CDO->MaxRange;
                AttackData.MinRange = CDO->MinRange;
            }
        }
    }
};

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "If true, the movement chain adapts its direction based on the player’s last move direction."))
    bool bUseDynamicDirectionFromPlayer = false;

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

protected:
	UAC_BehaviorDecision();

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnTargetDetected(AActor* Target);

    UPROPERTY(EditDefaultsOnly, Category = "UAC_BehaviorDecision")
    UAttackAbilityAsset* AttackAbilityAsset;

    UPROPERTY(EditDefaultsOnly, Category = "UAC_BehaviorDecision")
    UAttackAbilityMovementChainMapAsset* AttackAbilityMovementChainMapAsset;

    UPROPERTY(EditDefaultsOnly)
    float SecondsCheckMovement = 1.0f;

    UPROPERTY(EditDefaultsOnly)
    bool EnableSelectedDebug = false;

    UPROPERTY(EditDefaultsOnly)
    bool EnableAllDataDebug = false;

public:
    UFUNCTION(BlueprintCallable)
    FAttackData GetBestAttack(float DistanceToTarget);

    UFUNCTION(BlueprintCallable)
    TArray<FMovementAbilityData> GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

    FAttackData LastSelectedAttackAbilityData;

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForSelectedAttackAbility(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass) const;

    UPROPERTY(EditDefaultsOnly)
    EBehaviorState BehaviorState = EBehaviorState::None;

    float CalculateAttackAbilityScoreBasedOnTargetDistance(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange);

    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainAsset* MovementChainAsset);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainAsset* MovementChainAsset);

    bool ApplyDirectionPoliciesToSelectedMovementChain(UMovementChainAsset* SelectedMovementChainAsset);

    FGameplayTag GetRandomDirectionTag();

    float GetTargetDistance();

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_HeroMovementListener* HeroMovementListenerComp;
};
