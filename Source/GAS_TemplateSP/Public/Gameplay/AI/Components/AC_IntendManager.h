// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "AC_IntendManager.generated.h"


class UMovementChainData;
struct FMovementChainEndData;

UCLASS()
class GAS_TEMPLATESP_API UAC_IntendManager : public UAC_AIControllerBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    EEnemyIntent GetCurrentIntent() const { return CurrentIntent; }

    void OnMovementChainCompleted(const FMovementChainEndData& MovementChainEndData);

    void OnMovementChainAborted(const UMovementChainData* Chain);

private:
    void OnMovementChainEnded(const FMovementChainEndData& EndData);

    void EscalateIntent();

    void DeescalateIntent();

private:
    UPROPERTY()
    EEnemyIntent CurrentIntent = EEnemyIntent::LowPressure;
	

};
