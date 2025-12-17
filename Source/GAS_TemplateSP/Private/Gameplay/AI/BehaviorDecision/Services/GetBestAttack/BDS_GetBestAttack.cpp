// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/GetBestAttack/BDS_GetBestAttack.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"

void UBDS_GetBestAttack::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UAttackDataBase* UBDS_GetBestAttack::GetBestAttack()
{
    if (!IsValid(AttackAbilityAsset) || !EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackAbilityAsset or OwnerEnemyASC is null in: %s !"), *GetName());
        return nullptr;
    }

	float BestScore = -FLT_MAX;
	FAttackScoreDebug BestScoreDebug;
	UAttackDataBase* BestAttackData = nullptr;
	FAttackDecisionContext AttackDecisionContext = FAttackDecisionContext();

    for (UAttackDataBase* AttackData : AttackAbilityAsset->OptionalAttacks)
    {
		// ---------------- ENABLE CHECK ----------------
		FAttackEnableDebug EnableDebug;
		if (!AttackData->IsEnable(AttackDecisionContext, bEnableDebug ? &EnableDebug : nullptr))
		{
			if (bEnableDebug)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("UBDS_ComingAttackReactionBase: ReactionDisabled = %s | Reason = %s |"),
					*AttackData->AttackName.ToString(),
					*UEnum::GetValueAsString(EnableDebug.Reason)
				);
			}
			continue;
		}

		// ---------------- CHANCE CHECK ----------------
		FAttackChanceDebug ChanceDebug;
		if (!AttackData->PassesChance(AttackDecisionContext, bEnableDebug ? &ChanceDebug : nullptr))
		{
			if (bEnableDebug)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("UBDS_ComingAttackReactionBase: ReactionRollFailed = %s | Roll = %.2f Threshold = %.2f |"),
					*AttackData->AttackName.ToString(),
					ChanceDebug.Roll,
					ChanceDebug.Threshold
				);
			}
			continue;
		}

		// ---------------- SCORE ----------------
		FAttackScoreDebug ScoreDebug;
		const float Score = AttackData->GetScore(AttackDecisionContext, bEnableDebug ? &ScoreDebug : nullptr);
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("UBDS_ComingAttackReactionBase: ReactionScore = %s | Behavior = %.2f, Tag = %.2f, Bias = %.2f, Total = %.2f |"),
				*AttackData->AttackName.ToString(),
				ScoreDebug.BehaviorScore,
				ScoreDebug.ComboScore,
				ScoreDebug.Bias,
				ScoreDebug.Total
			);
		}

		// ---------------- BEST PICK ----------------
		if (Score > BestScore)
		{
			BestScore = Score;
			BestAttackData = AttackData;
			BestScoreDebug = ScoreDebug;
		}
    }

	return BestAttackData;
}


