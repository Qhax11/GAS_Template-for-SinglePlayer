// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "AC_BehaviorDecision.generated.h"


USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UGA_MeleeAttackBase> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag AbilityCooldownTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownPenalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CounterPenalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoreBias;
};

UCLASS(BlueprintType)
class UAttackAbilityDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAttackData> AttackAbilities;
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
    Walk         UMETA(DisplayName = "Walk"),
    Run          UMETA(DisplayName = "Run"),
    Dash         UMETA(DisplayName = "Dash")
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
struct FMovementData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FName MovementName = NAME_None;

    UPROPERTY(EditDefaultsOnly)
    EMovementType MovementType;

    UPROPERTY(EditDefaultsOnly)
    EMovementDirection Direction;

    UPROPERTY(EditDefaultsOnly)
    UCurveFloat* DistanceScoreCurve;

    // Increases the movement score based on the hero’s most recent movement direction (e.g. if the hero moved left, movements matching that direction receive a bonus).
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Score modifier based on the hero's last input direction. Used to react to player's movement more responsively."))
    TMap<EHeroRelativeDirection, float> HeroRelativeDirectionScoreModifiers;

    // Increases the movement score if the previous AI movement was in the same or similar direction (e.g. continuing a dash in the same direction).
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Score modifier based on the direction of the last selected AI movement. Encourages directional consistency, especially after dashes."))
    TMap<EMovementDirection, float> LastMovementDirectionScoreModifiers;

    UPROPERTY(EditDefaultsOnly)
    float ScoreModifierWhenTargetIsMoving = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    float ScoreModifierWhenTargetIsIdle = 0.0f;

    UPROPERTY(EditDefaultsOnly, Transient, meta = (ToolTip = "Only used if MovementType is Dash or any movement ability."))
    bool bIsDashType = false;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsDashType"))
    FGameplayTag MovementCooldownTag;

    UPROPERTY(EditDefaultsOnly)
    float ScoreBias = 0.0f;
};

UCLASS(BlueprintType)
class UMovementDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMovementData> Movements;
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

    UPROPERTY(EditDefaultsOnly)
    UAttackAbilityDataAsset* AttackAbilityDataAsset;

    UPROPERTY(EditDefaultsOnly)
    UMovementDataAsset* MovementDataAsset;

    UPROPERTY(EditDefaultsOnly)
    float SecondsCheckMovement = 1.0f;

    UPROPERTY(EditDefaultsOnly)
    bool EnableDebug = false;
public:
    UFUNCTION(BlueprintCallable)
    FAttackData GetBestAttack(float DistanceToTarget);

    UFUNCTION(BlueprintCallable)
    FMovementData GetBestMovement(float DistanceToTarget, FAttackData SelectedAttackAbilityData);

    FAttackData LastSelectedAttackAbilityData;
    FMovementData LastSelectedMovementData;

private:
    float CalculateAttackAbilityScoreBasedOnTargetDistance(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange);

    float CalculateMovementScoreBasedOnTargetDistance(float DistanceToTarget, FMovementData MovementData, FAttackData SelectedAttackAbilityData);

    float CalculateMovementScoreBasedOnTargetMovement(FMovementData MovementData, FAttackData SelectedAttackAbilityData);

    float CalculateMovementChainScoreBasedOnLastSelectedMovement(FMovementData MovementData, FMovementData LastMovementData);

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_HeroMovementListener* HeroMovementListenerComp;
};
