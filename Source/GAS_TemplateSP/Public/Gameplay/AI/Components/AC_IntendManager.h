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

protected:
    void OnMovementChainCompleted(const FMovementChainEndData& EndData);

    void OnEnemyComboChainCompleted(const FEnemyComboChainEndData& EndData);

private:
    void IncreasePressure();

    void DecreasePressure();

    UPROPERTY()
    EEnemyIntent CurrentIntent = EEnemyIntent::LowPressure;

    UPROPERTY()
    UAC_EnemyMovementManager* MovementManager;

    UPROPERTY()
    UAC_EnemyMeleeComboManager* MeleeComboManager;
};
