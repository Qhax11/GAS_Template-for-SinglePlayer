// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "ComingAttackReactionDataTakeHit.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataTakeHit : public UComingAttackReactionData
{
	GENERATED_BODY()

public:
	UComingAttackReactionDataTakeHit();

	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug = nullptr) const override;
	
};
