// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataParry.h"

UComingAttackReactionDataParry::UComingAttackReactionDataParry()
{
	ComingAttackReactionName = FName("Parry");
	ReactionType = EComingAttackReaction::Parry;
	MinimumTimeBeforeHitToReact = 0.2f;	
	PreferredTriggerTimeBeforeHit = 0.3f;	
	BaseChance = 0.7f;
}

bool UComingAttackReactionDataParry::IsEnable(FComingAttackPayload ComingAttackPayload) const
{
	bool bIsUnparryableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	return Super::IsEnable(ComingAttackPayload) && !bIsUnparryableAttack;
}

bool UComingAttackReactionDataParry::PassesChanceRoll(const UAbilitySystemComponent* ASC) const
{
    const UAS_Base* BaseAttributes = ASC ? ASC->GetSet<UAS_Base>() : nullptr;
    if (!BaseAttributes)
    {
        return false;
    }

    const float Posture = BaseAttributes->GetPosture();
    const float MaxPosture = BaseAttributes->GetMaxPosture();
    const float NormalizedPosture = FMath::Clamp(Posture / MaxPosture, 0.f, 1.f);

    // Sekiro mantýðý: posture yükseldikçe parry zorlaþýr
    const float FinalChance = BaseChance * (1.f - NormalizedPosture);

    const float Roll = FMath::FRandRange(0.f, 1.f);
    return Roll <= FinalChance;
}

float UComingAttackReactionDataParry::GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug) const
{
    float Score = Super::GetScore(ComingAttackPayload, BehaviorState);

    return Score;
}

