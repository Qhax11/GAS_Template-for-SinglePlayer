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

	virtual bool IsEnable(UComingAttackReactionData* ComingReactionData, FComingAttackPayload ComingAttackPayload) const override;

};
