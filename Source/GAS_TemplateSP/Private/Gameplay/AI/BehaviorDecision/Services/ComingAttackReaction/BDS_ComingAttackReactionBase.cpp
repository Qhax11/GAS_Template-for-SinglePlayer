// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include <Gameplay/Attributes/AS_Base.h>

void UBDS_ComingAttackReactionBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UComingAttackReactionData* UBDS_ComingAttackReactionBase::GetBestComingAttackReaction(FComingAttackPayload& ComingAttackPayload)
{
	if (!IsValid(ComingAttackReactionAsset) || !ComingAttackPayload.ComingAttack || !EnemyASC)
	{
		return nullptr;
	}

	UComingAttackReactionData* BestReaction = nullptr;
	FReactionScoreDebug BestScoreDebug;
	float BestScore = -FLT_MAX;

	for (UComingAttackReactionData* Reaction : ComingAttackReactionAsset->ComingAttackReactions)
	{
		if (!IsValid(Reaction))
		{
			continue;
		}

		// ---------------- ENABLE CHECK ----------------
		FReactionEnableDebug EnableDebug;
		if (!Reaction->IsEnable(ComingAttackPayload, bEnableDebug ? &EnableDebug : nullptr))
		{
			if (bEnableDebug)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("UBDS_ComingAttackReactionBase: ReactionDisabled = %s | Reason = %s |"),
					*Reaction->ComingAttackReactionName.ToString(),
					*UEnum::GetValueAsString(EnableDebug.Reason)
				);
			}
			continue;
		}

		// ---------------- CHANCE CHECK ----------------
		FReactionChanceDebug ChanceDebug;
		if (!Reaction->PassesChanceRoll(EnemyASC, bEnableDebug ? &ChanceDebug : nullptr))
		{
			if (bEnableDebug)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("UBDS_ComingAttackReactionBase: ReactionRollFailed = %s | Reason = %s | Roll = %.2f Threshold = %.2f |"),
					*Reaction->ComingAttackReactionName.ToString(),
					*UEnum::GetValueAsString(ChanceDebug.Reason),
					ChanceDebug.Roll,
					ChanceDebug.Threshold
				);
			}
			continue;
		}

		// ---------------- SCORE ----------------
		FReactionScoreDebug ScoreDebug;
		const float Score = Reaction->GetScore(ComingAttackPayload, BehaviorState, bEnableDebug ? &ScoreDebug : nullptr);
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("UBDS_ComingAttackReactionBase: ReactionScore = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
				*Reaction->ComingAttackReactionName.ToString(),
				ScoreDebug.BehaviorStateScore,
				ScoreDebug.TagScore,
				ScoreDebug.Bias,
				ScoreDebug.Total
			);
		}

		// ---------------- BEST PICK ----------------
		if (Score > BestScore)
		{
			BestScore = Score;
			BestReaction = Reaction;
			BestScoreDebug = ScoreDebug;
		}
	}

	// ---------------- WINNER DEBUG ----------------
	if (bEnableDebug && BestReaction)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UBDS_ComingAttackReactionBase: WINNER = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
			*BestReaction->ComingAttackReactionName.ToString(),
			BestScoreDebug.BehaviorStateScore,
			BestScoreDebug.TagScore,
			BestScoreDebug.Bias,
			BestScoreDebug.Total
		);
	}

	return BestReaction;
}







