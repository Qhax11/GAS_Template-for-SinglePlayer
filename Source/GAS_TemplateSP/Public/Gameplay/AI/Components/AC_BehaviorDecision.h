// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
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
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag AbilityCooldownTag;

    UPROPERTY(EditDefaultsOnly)
    TMap<EBehaviorState, float> BehaviorStateScoreModifiers;

    UPROPERTY(EditDefaultsOnly)
    float MinRange;

    UPROPERTY(EditDefaultsOnly)
    float MaxRange;

    UPROPERTY(EditDefaultsOnly)
    float CounterPenalty;

    UPROPERTY(EditDefaultsOnly)
    float ScoreBias;
};

UCLASS(BlueprintType)
class UAttackAbilityAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackData> AttackAbilities;
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
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag AbilityTriggerTag;

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gameplay.Utilities.Direction"))
    FGameplayTag DirectionTag;
};

UCLASS(BlueprintType)
class UMovementChainAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName MovementChainName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FMovementAbilityData> MovementChain;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EBehaviorState, float> BehaviorStateModifiers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCurveFloat* DistanceScoreCurve = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bUseDynamicDirectionFromPlayer = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ScoreModifierWhenTargetIsMoving = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ScoreModifierWhenTargetIsIdle = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MinRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
    TArray<FAttackAbilityMovementChain> ChainMappings;
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
    TArray<FMovementAbilityData> GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> AttackAbility);

    FAttackData LastSelectedAttackAbilityData;

protected:
    TArray<UMovementChainAsset*> GetMovementChainsForAbility(TSubclassOf<UGAS_GameplayAbilityBase> Ability) const;

    UPROPERTY(EditDefaultsOnly)
    EBehaviorState BehaviorState = EBehaviorState::None;

    float CalculateAttackAbilityScoreBasedOnTargetDistance(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange);

    float CalculateMovementChainScoreBasedOnTargetDistance(UMovementChainDataAsset* MovementChain);

    float CalculateMovementChainScoreBasedOnTargetMovement(UMovementChainDataAsset* MovementChain);

    float CalculateMovementChainScoreBasedOnBehaviorState(UMovementChainDataAsset* MovementChain);

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_HeroMovementListener* HeroMovementListenerComp;
};
