// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BDS_ComingAttackReaction.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

void UBDS_ComingAttackReaction::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
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

EComingAttackReaction UBDS_ComingAttackReaction::GetComingAttackDecision(FComingAttackPayload ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionAsset) || !ComingAttackPayload.ComingAttack)
    {
        return EComingAttackReaction::TakeDamage;
    }

    EComingAttackReaction BestReaction = EComingAttackReaction::TakeDamage;
    float BestScore = -FLT_MAX;

    for (const FComingAttackReactionData& ReactionData : ComingAttackReactionAsset->ComingAttackReactions)
    {
        if (!PassesFinalChanceRoll(ReactionData))
        {
            continue;
        }

        float BehaviorScore = CalculateBehaviorStateScore(ReactionData);
        float TagScore = CalculateTagScore(ReactionData, ComingAttackPayload);

        float TotalScore = BehaviorScore + TagScore + ReactionData.ScoreBias;

        UE_LOG(LogTemp, Log, TEXT("[AI] Reaction %s (%d) → Score: %.2f"), *ReactionData.ComingAttackReactionName.ToString(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestReaction = ReactionData.ReactionType;
        }
    }

    return BestReaction;
}

float UBDS_ComingAttackReaction::CalculateBehaviorStateScore(const FComingAttackReactionData& Data) const
{
	if (const float* Mod = Data.BehaviorStateScoreModifiers.Find(BehaviorState))
	{
		return *Mod;
	}

	return 0.f;
}

float UBDS_ComingAttackReaction::CalculateTagScore(const FComingAttackReactionData& Data, const FComingAttackPayload ComingAttackPayload) const
{
	float Score = 0.f;

	for (const auto& Pair : Data.TagScoreModifiers)
	{
		if (ComingAttackPayload.ComingAttackTags.HasTag(Pair.Key))
		{
			Score += Pair.Value;
		}
	}

	return Score;
}

bool UBDS_ComingAttackReaction::PassesChanceRoll(const FComingAttackReactionData& ReactionData) const
{
    const float Roll = FMath::FRandRange(0.f, 1.f);  // 0–1 arası
    const bool bPassed = Roll <= ReactionData.BaseChance;

    UE_LOG(LogTemp, Log, TEXT("[AI] Static chance reaction %s: roll %.2f <= base chance %.2f → %s"),
        *ReactionData.ComingAttackReactionName.ToString(),
        Roll,
        ReactionData.BaseChance,
        bPassed ? TEXT("PASS") : TEXT("FAIL"));

    return bPassed;
}

bool UBDS_ComingAttackReaction::PassesChanceRollBasedOnPosture(const FComingAttackReactionData& ReactionData) const
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
        *ReactionData.ComingAttackReactionName.ToString(),
        Roll,
        PostureValue,
        bPassed ? TEXT("PASS") : TEXT("FAIL"));

    return bPassed;
}

bool UBDS_ComingAttackReaction::PassesFinalChanceRoll(const FComingAttackReactionData& ReactionData) const
{
    if (ReactionData.ReactionType == EComingAttackReaction::Parry)
    {
        return PassesChanceRollBasedOnPosture(ReactionData);
    }
    else if(ReactionData.ReactionType == EComingAttackReaction::Dodge)
    {
        return PassesChanceRoll(ReactionData);
    }
    else
    {
        return true;
    }
}

