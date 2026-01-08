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
        UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestAttack: AttackAbilityAsset or OwnerEnemyASC is null in: %s !"), *GetName());
        return nullptr;
    }

	float BestScore = -FLT_MAX;
	FAttackScoreDebug BestScoreDebug;
	UAttackDataBase* BestAttackData = nullptr;

	FAttackDecisionContext AttackDecisionContext;
	AttackDecisionContext.EnemyIntent = IntendManager->GetCurrentIntent();
	AttackDecisionContext.Owner = Enemy;
	AttackDecisionContext.Target = Hero;
	AttackDecisionContext.OwnerASC = EnemyASC;

	for (UAttackDataBase* AttackData : AttackAbilityAsset->OptionalAttacks)
	{
		if (!AttackData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestAttack: AttackData is null!"));
			continue;
		}

		// ---------------- ENABLE CHECK ----------------
		FAttackEnableDebug EnableDebug;
		if (!AttackData->IsEnable(AttackDecisionContext, bEnableDebug ? &EnableDebug : nullptr))
		{
			if (bEnableDebug)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("Decision: Service: UBDS_GetBestAttack: AttackDisabled = %s | Reason = %s |"),
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
					TEXT("Decision: Service: UBDS_GetBestAttack: AttackPassesChangeFailed = %s | Roll = %.2f Threshold = %.2f |"),
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
				TEXT("Decision: Service: UBDS_GetBestAttack: AttackScore = %s | Intent = %.2f, Distance = %.2f, Bias = %.2f, Total = %.2f |"),
				*AttackData->AttackName.ToString(),
				ScoreDebug.IntentScore,
				ScoreDebug.DistanceScore,
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

	// ---------------- WINNER DEBUG ----------------
	if (bEnableDebug )
	{
		if (BestAttackData) 
		{
			UE_LOG(LogTemp, Warning,
				TEXT("Decision: Service: UBDS_GetBestAttack: WINNER: %s | Current intent: %s | Score: Intent = %.2f, Distance = %.2f, Bias = %.2f, Total = %.2f |"),
				*BestAttackData->AttackName.ToString(),
				*UEnum::GetValueAsString(BestScoreDebug.CurrentIntent),
				BestScoreDebug.IntentScore,
				BestScoreDebug.DistanceScore,
				BestScoreDebug.Bias,
				BestScoreDebug.Total
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Decision: Service: UBDS_GetBestAttack: No valid attack found!"));
		}
	}

	return BestAttackData;
}


