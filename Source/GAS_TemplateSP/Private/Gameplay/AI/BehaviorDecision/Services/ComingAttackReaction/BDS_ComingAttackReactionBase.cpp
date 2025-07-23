// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"

void UBDS_ComingAttackReactionBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);

    UComingAttackReactionAsset* CastedAsset = Cast<UComingAttackReactionAsset>(BehaviorServiceInitParams.Asset);
    if (CastedAsset)
    {
        ComingAttackReactionAsset = CastedAsset;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid asset type passed to UComingAttackReactionService!"));
    }
}

float UBDS_ComingAttackReactionBase::CalculateComingAttackReactionScore(FComingAttackPayload ComingAttackPayload)
{
    float BehaviorScore = CalculateBehaviorStateScore();
    float TagScore = CalculateTagScore(ComingAttackPayload);

    float TotalScore = BehaviorScore + TagScore + ScoreBias;
    UE_LOG(LogTemp, Log, TEXT("[AI] Reaction %s → Score: %.2f"), *ComingAttackReactionName.ToString(), TotalScore);

    return TotalScore;
}

bool UBDS_ComingAttackReactionBase::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
    return PassesFinalChanceRoll() && ComingAttackPayload.ComingAttackHitTime > MinimumTimeBeforeHitToReact;
}

float UBDS_ComingAttackReactionBase::CalculateBehaviorStateScore() const
{
	if (const float* Mod = BehaviorStateScoreModifiers.Find(BehaviorState))
	{
		return *Mod;
	}

	return 0.f;
}

float UBDS_ComingAttackReactionBase::CalculateTagScore(const FComingAttackPayload ComingAttackPayload) const
{
	float Score = 0.f;

	for (const auto& Pair : TagScoreModifiers)
	{
		if (ComingAttackPayload.ComingAttackTags.HasTag(Pair.Key))
		{
			Score += Pair.Value;
		}
	}

	return Score;
}

bool UBDS_ComingAttackReactionBase::PassesChanceRoll() const
{
    const float Roll = FMath::FRandRange(0.f, 1.f);  
    const bool bPassed = Roll <= BaseChance;

    UE_LOG(LogTemp, Log, TEXT("[AI] Static chance reaction %s: roll %.2f <= base chance %.2f → %s"),
        *ComingAttackReactionName.ToString(),
        Roll,
        BaseChance,
        bPassed ? TEXT("PASS") : TEXT("FAIL"));

    return bPassed;
}

bool UBDS_ComingAttackReactionBase::PassesChanceRollBasedOnPosture() const
{
    UAS_Base* BaseAttributes = const_cast<UAS_Base*>(EnemyASC->GetSet<UAS_Base>());
    if (!BaseAttributes)
    {
        return false;
    }

    const float PostureValue = BaseAttributes->GetPosture();  
    const float Roll = FMath::FRandRange(0.f, 100.f);

    const bool bPassed = Roll <= PostureValue;

    UE_LOG(LogTemp, Log, TEXT("[AI] Posture-based reaction %s: roll %.2f <= posture %.2f → %s"),
        *ComingAttackReactionName.ToString(),
        Roll,
        PostureValue,
        bPassed ? TEXT("PASS") : TEXT("FAIL"));

    return bPassed;
}

bool UBDS_ComingAttackReactionBase::PassesFinalChanceRoll() const
{
    if (ReactionType == EComingAttackReaction::Parry)
    {
        return PassesChanceRollBasedOnPosture();
    }
    else if(ReactionType == EComingAttackReaction::Dodge)
    {
        return PassesChanceRoll();
    }
    else
    {
        return true;
    }
}

