// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/GetBestSequence/BDS_GetBestSequence.h"

void UBDS_GetBestSequence::Initialize(const FBehaviorServiceInitParams& InitParams)
{
	Super::Initialize(InitParams);
}

UAttackSequenceAsset* UBDS_GetBestSequence::GetBestSequence()
{
	return ScoreAndSelectBestSequence();
}

UAttackSequenceAsset* UBDS_GetBestSequence::ScoreAndSelectBestSequence() const
{
    if (AvailableSequences.Num() == 0)
    {
        return nullptr;
    }

    UAttackSequenceAsset* BestSequence = nullptr;
    float BestScore = -1.0f;

    for (UAttackSequenceAsset* Sequence : AvailableSequences)
    {
        if (!IsSequenceValid(Sequence))
        {
            continue;
        }

        float Score = CalculateSequenceScore(Sequence);

        if (Score > BestScore)
        {
            BestScore = Score;
            BestSequence = Sequence;
        }
    }

    return BestSequence;
}

float UBDS_GetBestSequence::CalculateSequenceScore(UAttackSequenceAsset* Sequence) const
{
    if (!Sequence || !EnemyController)
    {
        return 0.0f;
    }

    float Score = Sequence->BasePriority;

    // Distance scoring
    float Distance = EnemyController->GetTargetHeroDistance();
    if (Sequence->DistanceScoreCurve)
    {
        Score *= Sequence->DistanceScoreCurve->GetFloatValue(Distance);
    }

    return Score;
}

bool UBDS_GetBestSequence::IsSequenceValid(UAttackSequenceAsset* Sequence) const
{
    if (!Sequence || Sequence->Steps.Num() == 0 || !EnemyController)
    {
        return false;
    }

    // Distance constraints
    float Distance = EnemyController->GetTargetHeroDistance();
    if (Distance < Sequence->MinDistanceToStart || Distance > Sequence->MaxDistanceToStart)
    {
        return false;
    }

    // First step must have valid attack
    if (!Sequence->Steps[0].AttackToExecute.AbilityClass)
    {
        return false;
    }

    return true;
}
