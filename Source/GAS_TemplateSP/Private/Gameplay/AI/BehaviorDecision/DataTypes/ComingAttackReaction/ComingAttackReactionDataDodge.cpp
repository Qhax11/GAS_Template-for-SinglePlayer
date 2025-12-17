// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionDataDodge.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"

UComingAttackReactionDataDodge::UComingAttackReactionDataDodge()
{
    ComingAttackReactionName = FName("Dodge");
    ReactionType = EComingAttackReaction::Dodge;

    MinimumTimeBeforeHitToReact = 0.f;    
    PreferredTriggerTimeBeforeHit = 0.2f;    

	BaseChance = 0.65f;
	MinChance = 0.35f;
	MaxChance = 0.90f;

	ScoreBias = 0.1f;

	DodgeMovementAbilityData = CreateDefaultSubobject<UMovementSingleData>(TEXT("DodgeMovementAbilityData"));

    // Dash distance
    DodgeMovementAbilityData->AbilityTriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dodge;
	DodgeMovementAbilityData->DirectionPolicyTag = GAS_Tags::TAG_AI_Direction_Policy_EscapeFromAttack;
	DodgeMovementAbilityData->ResolvedDirectionTag = GAS_Tags::TAG_Gameplay_Direction_Backward;
	DodgeMovementAbilityData->EnableDirectionPolicy = true;
    DodgeMovementAbilityData->AbilityEventMagnitude = 300.0f;
}

bool UComingAttackReactionDataDodge::IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug) const
{
	if (!Super::IsEnable(ComingAttackPayload, OutDebug))
	{
		return false;
	}

	const bool bIsInActiveAttackPhase = ComingAttackPayload.DefenderASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	if (bIsInActiveAttackPhase)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::InActiveAttackPhase;
		}
		return false;
	}

	const bool bIsUndodgeable = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
	if (bIsUndodgeable)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EReactionDisableReason::UndodgeableAttack;
		}
		return false;
	}

	return true;
}

bool UComingAttackReactionDataDodge::PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug) const
{
	const float BaseDodgeChance = BaseChance;      
	const float MinDodgeChance  = MinChance;      
	const float MaxDodgeChance  = MaxChance;     

	float DodgeChance = BaseDodgeChance;

	// Posture bilgisi varsa posture'a göre ayarla
	if (ASC)
	{
		if (const UAS_Base* BaseAttributes = ASC->GetSet<UAS_Base>())
		{
			const float Posture = BaseAttributes->GetPosture();
			const float MaxPosture = BaseAttributes->GetMaxPosture();

			if (MaxPosture > 0.f)
			{
				// Posture düþtükçe dodge daha güvenli
				const float NormalizedPosture = FMath::Clamp(Posture / MaxPosture, 0.f, 1.f);

				// Posture düþük ? bonus yüksek
				const float LowPostureBonus = (1.f - NormalizedPosture) * 0.25f;
				DodgeChance += LowPostureBonus;
			}
		}
	}

	DodgeChance = FMath::Clamp(DodgeChance, MinDodgeChance, MaxDodgeChance);

	const float Roll = FMath::FRandRange(0.f, 1.f);
	const bool bPassed = Roll <= DodgeChance;

	if (OutDebug)
	{
		OutDebug->Roll = Roll;
		OutDebug->Threshold = DodgeChance;
		OutDebug->Reason = bPassed
			? EReactionChanceFailReason::None
			: EReactionChanceFailReason::RandomRollFailed;
	}

	return bPassed;
}

