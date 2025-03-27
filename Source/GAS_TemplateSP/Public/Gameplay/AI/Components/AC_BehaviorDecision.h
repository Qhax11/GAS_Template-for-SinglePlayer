// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_MovementListener.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MovementName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMovementType MovementType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMovementDirection Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EHeroRelativeDirectionToTarget, float> HeroRelativeDirectionToTargetScoreModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoreModifierWhenTargetIsMoving = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoreModifierWhenTargetIsIdle = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
    bool EnableDebug = false;
public:
    UFUNCTION(BlueprintCallable)
    FAttackData GetBestAttack(float DistanceToTarget);

    UFUNCTION(BlueprintCallable)
    FMovementData GetBestMovement(float DistanceToTarget, FAttackData SelectedAttackAbilityData);

private:
    float CalculateAttackAbilityScoreBasedOnTargetDistance(float DistanceToTarget, float AbilityMinRange, float AbilityMaxRange);

    float CalculateMovementScoreBasedOnTargetDistance(float DistanceToTarget, FMovementData MovementData, FAttackData SelectedAttackAbilityData);

    float CalculateMovementScoreBasedOnTargetMovement(FMovementData MovementData, FAttackData SelectedAttackAbilityData);

    FAttackData LastSelectedAttackAbilityData;
    FMovementData LastSelectedMovementyData;

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_MovementListener* HeroMovementListenerComp;

};
