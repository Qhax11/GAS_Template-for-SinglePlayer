// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataParry.h"

UComingAttackReactionDataParry::UComingAttackReactionDataParry()
{
	ComingAttackReactionName = FName("Parry");
	ReactionType = EComingAttackReaction::Parry;
	MinimumTimeBeforeHitToReact = 0.2f;	
	PreferredTriggerTimeBeforeHit = 0.3f;	

	BaseChance = 0.5f;   
	MinChance = 0.15f;  
	MaxChance = 0.95f;  
}

bool UComingAttackReactionDataParry::IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug) const
{
	if (!Super::IsEnable(ComingAttackPayload, OutDebug))
	{
		return false; // reason base tarafýndan yazýldý
	}

	const bool bIsUnparryable = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	if (bIsUnparryable)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::UnparryableAttack;
		}
		return false;
	}

	return true;
}

bool UComingAttackReactionDataParry::PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug) const
{
    const UAS_Base* BaseAttributes = ASC ? ASC->GetSet<UAS_Base>() : nullptr;
    if (!BaseAttributes)
    {
        return false;
    }

    const float Posture = BaseAttributes->GetPosture();
    const float MaxPosture = BaseAttributes->GetMaxPosture();
	const float Threshold = FMath::Clamp(Posture / MaxPosture, 0.15f, 0.95f);
	const float Roll = FMath::FRandRange(0.f, 1.f);

	const bool bPassed = Roll <= Threshold;

	if (OutDebug)
	{
		OutDebug->Roll = Roll;
		OutDebug->Threshold = Threshold;
		OutDebug->Reason = bPassed
			? EReactionChanceFailReason::None
			: EReactionChanceFailReason::PostureRollFailed;
	}

	return bPassed;
}

float UComingAttackReactionDataParry::GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug) const
{
    float Score = Super::GetScore(ComingAttackPayload, BehaviorState);

    return Score;
}

