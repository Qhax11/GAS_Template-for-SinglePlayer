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

	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload) const override;

	virtual bool PassesChanceRoll(const UAbilitySystemComponent* ASC) const override;

	virtual float GetScore(const FComingAttackPayload& ComingAttackPayload, EBehaviorState BehaviorState, FReactionScoreDebug* OutDebug = nullptr) const override;

};
