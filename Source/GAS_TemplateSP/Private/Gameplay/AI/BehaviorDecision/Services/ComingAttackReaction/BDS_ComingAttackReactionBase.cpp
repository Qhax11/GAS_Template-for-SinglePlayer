// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include <Gameplay/Attributes/AS_Base.h>
#include <Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h>

void UBDS_ComingAttackReactionBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);
}

UComingAttackReactionData* UBDS_ComingAttackReactionBase::GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload)
{
    if (!IsValid(ComingAttackReactionAsset) || !ComingAttackPayload.ComingAttack)
    {
        return nullptr;
    }

    UComingAttackReactionData* BestReaction = nullptr;
    float BestScore = -FLT_MAX;

    for (UComingAttackReactionData* Reaction : ComingAttackReactionAsset->ComingAttackReactions)
    {
        if (!Reaction->IsEnable(Reaction, ComingAttackPayload))
        {
            continue;
        }

        float BehaviorScore = CalculateBehaviorStateScore(Reaction);
        float TagScore = CalculateTagScore(Reaction, ComingAttackPayload);

        float ComingAttackReactionScore = BehaviorScore + TagScore + Reaction->ScoreBias;

        if (ComingAttackReactionScore > BestScore)
        {
            BestScore = ComingAttackReactionScore;
            BestReaction = Reaction;
        }
    }

    if (BestReaction) 
    {
        if (BestReaction->ReactionType == EComingAttackReaction::Dodge)
        {
            UComingAttackReactionDataDodge* ComingAttackReactionDataDodge = Cast<UComingAttackReactionDataDodge>(BestReaction);
            ComingAttackReactionDataDodge->ApplyDirectionPoliciesToMovementAbility(HeroMovementListenerComp);
        }
    }

    return BestReaction;
}

float UBDS_ComingAttackReactionBase::CalculateBehaviorStateScore(UComingAttackReactionData* ComingReactionData) 
{
	if (const float* Mod = ComingReactionData->BehaviorStateScoreModifiers.Find(BehaviorState))
	{
		return *Mod;
	}

	return 0.f;
}

float UBDS_ComingAttackReactionBase::CalculateTagScore(UComingAttackReactionData* ComingReactionData, const FComingAttackPayload ComingAttackPayload) 
{
	float Score = 0.f;

	for (const auto& Pair : ComingReactionData->TagScoreModifiers)
	{
		if (ComingAttackPayload.ComingAttackTags.HasTag(Pair.Key))
		{
			Score += Pair.Value;
		}
	}

	return Score;
}

bool UBDS_ComingAttackReactionBase::PassesFinalChanceRoll(UComingAttackReactionData* ComingReactionData) 
{
    if (ComingReactionData->ReactionType == EComingAttackReaction::Dodge)
    {
        return PassesChanceRoll(ComingReactionData);
    }
    else if (ComingReactionData->ReactionType == EComingAttackReaction::Parry)
    {
        return PassesChanceRollBasedOnPosture(ComingReactionData);
    }
    // ReactionType == EComingAttackReaction::TakeDamage
    else 
    {
        return true;
    }
}

bool UBDS_ComingAttackReactionBase::PassesChanceRoll(UComingAttackReactionData* ComingReactionData) 
{
    const float Roll = FMath::FRandRange(0.f, 1.f);  
    const bool bPassed = Roll <= ComingReactionData->BaseChance;

    return bPassed;
}

bool UBDS_ComingAttackReactionBase::PassesChanceRollBasedOnPosture(UComingAttackReactionData* ComingReactionData) 
{
    UAS_Base* BaseAttributes = const_cast<UAS_Base*>(EnemyASC->GetSet<UAS_Base>());
    if (!BaseAttributes)
    {
        return false;
    }

    const float PostureValue = BaseAttributes->GetPosture();  
    const float Roll = FMath::FRandRange(0.f, 100.f);
    const bool bPassed = Roll <= PostureValue;

    return bPassed;
}



