// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackSequenceData.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "AC_SequenceExecutor.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_SequenceExecutor : public UAC_AIControllerBase
{
	GENERATED_BODY()
	
public:
    UAC_SequenceExecutor();

    // Start executing a sequence
    void BeginSequence(UAttackSequenceAsset* Sequence);

    // Get the current step's attack data
    const FSequenceStep* GetCurrentStep() const;

    // Advance to next step
    void AdvanceToNextStep();

    // Check if sequence is complete
    bool IsSequenceComplete() const;

    // Interrupt current sequence (for incoming attacks)
    void InterruptSequence();

    // Resume interrupted sequence
    void ResumeSequence();

    // Clear/reset sequence
    void ClearSequence();

    // Getters
    UFUNCTION(BlueprintCallable)
    bool HasActiveSequence() const { return CurrentSequence != nullptr; }

    UFUNCTION(BlueprintCallable)
    bool WasInterrupted() const { return bInterrupted; }

    UFUNCTION(BlueprintCallable)
    UMovementChainAsset* GetApproachMovement() const;

protected:
    UPROPERTY()
    UAttackSequenceAsset* CurrentSequence = nullptr;

    UPROPERTY()
    int32 CurrentStepIndex = 0;

    UPROPERTY()
    bool bInterrupted = false;

    UPROPERTY()
    int32 InterruptedStepIndex = 0;

    FTimerHandle WaitTimerHandle;
};
