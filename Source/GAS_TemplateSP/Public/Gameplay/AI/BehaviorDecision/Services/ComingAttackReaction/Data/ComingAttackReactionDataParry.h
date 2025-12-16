// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "ComingAttackReactionDataParry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataParry : public UComingAttackReactionData
{
	GENERATED_BODY()

public:
	UComingAttackReactionDataParry();

	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug = nullptr) const override;

	virtual bool PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug = nullptr) const override;

	virtual float GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug = nullptr) const override;
};
