// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/ComingAttackReactionData.h"
#include "ComingAttackReactionDataParry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataParry : public UComingAttackReactionData
{
	GENERATED_BODY()

public:
	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload) const override;
	
};
