// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_SequenceExecutor.h"

UAC_SequenceExecutor::UAC_SequenceExecutor()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAC_SequenceExecutor::BeginSequence(UAttackSequenceAsset* Sequence)
{
    if (!Sequence || Sequence->Steps.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid sequence passed to BeginSequence"));
        return;
    }

    CurrentSequence = Sequence;
    CurrentStepIndex = 0;
    bInterrupted = false;

#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Started Sequence: %s (%d steps)"),
        *Sequence->SequenceName.ToString(),
        Sequence->Steps.Num());
#endif
}

const FSequenceStep* UAC_SequenceExecutor::GetCurrentStep() const
{
    if (!CurrentSequence || CurrentStepIndex >= CurrentSequence->Steps.Num())
    {
        return nullptr;
    }

    return &CurrentSequence->Steps[CurrentStepIndex];
}

void UAC_SequenceExecutor::AdvanceToNextStep()
{
    if (!CurrentSequence)
    {
        return;
    }

    CurrentStepIndex++;

#if WITH_EDITOR
    if (CurrentStepIndex < CurrentSequence->Steps.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("Advanced to step %d/%d"),
            CurrentStepIndex + 1,
            CurrentSequence->Steps.Num());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Sequence complete: %s"),
            *CurrentSequence->SequenceName.ToString());
    }
#endif
}

bool UAC_SequenceExecutor::IsSequenceComplete() const
{
    return !CurrentSequence || CurrentStepIndex >= CurrentSequence->Steps.Num();
}

void UAC_SequenceExecutor::InterruptSequence()
{
    if (!CurrentSequence)
    {
        return;
    }

    bInterrupted = true;
    InterruptedStepIndex = CurrentStepIndex;

#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("Sequence interrupted at step %d/%d"),
        CurrentStepIndex + 1,
        CurrentSequence->Steps.Num());
#endif
}

void UAC_SequenceExecutor::ResumeSequence()
{
    if (!bInterrupted || !CurrentSequence)
    {
        return;
    }

    bInterrupted = false;
    // Resume from interrupted step (or next step, depending on design choice)
    CurrentStepIndex = InterruptedStepIndex;

#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Resuming sequence from step %d"), CurrentStepIndex + 1);
#endif
}

void UAC_SequenceExecutor::ClearSequence()
{
    CurrentSequence = nullptr;
    CurrentStepIndex = 0;
    bInterrupted = false;
    InterruptedStepIndex = 0;

    if (GetWorld()->GetTimerManager().IsTimerActive(WaitTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);
    }
}

UMovementChainAsset* UAC_SequenceExecutor::GetApproachMovement() const
{
    return CurrentSequence ? CurrentSequence->ApproachMovementChain : nullptr;
}