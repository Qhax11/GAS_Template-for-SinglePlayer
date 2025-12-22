// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "AC_IntendManager.generated.h"

class UAC_EnemyMovementManager;
class UAC_EnemyMeleeComboManager;
class UMovementChainData;
struct FMovementChainEndData;
struct FEnemyComboChainEndData;

UCLASS()
class GAS_TEMPLATESP_API UAC_IntendManager : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    EEnemyIntent GetCurrentIntent() const { return CurrentIntent; }

private:
    void OnMovementChainEnd(const FMovementChainEndData& EndData);

    void OnEnemyComboChainEnd(const FEnemyComboChainEndData& EndData);

private:
    void HandleMovementChainCompleted(const FMovementChainEndData& EndData);

    void HandleComboChainCompleted(const FEnemyComboChainEndData& EndData);

    void IncreasePressure();

    void DecreasePressure();

private:
    UPROPERTY()
    EEnemyIntent CurrentIntent = EEnemyIntent::LowPressure;

    UPROPERTY()
    UAC_EnemyMovementManager* MovementManager;

    UPROPERTY()
    UAC_EnemyMeleeComboManager* MeleeComboManager;

    int32 ConsecutiveSoftMovementCount = 0;
};
