// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "ComingAttackReactionDataDodge.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataDodge : public UComingAttackReactionData
{
	GENERATED_BODY()
	
public:
	//virtual void InitializeAfterSelection() override;

	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMovementAbilityData DodgeMovementAbilityData;
};
