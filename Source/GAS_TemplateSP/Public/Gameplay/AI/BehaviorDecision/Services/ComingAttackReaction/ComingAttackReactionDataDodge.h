// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/ComingAttackReactionData.h"
#include "ComingAttackReactionDataDodge.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataDodge : public UComingAttackReactionData
{
	GENERATED_BODY()
	
public:
	//virtual void InitializeAfterSelection() override;

	/*
	virtual bool IsEnable(FComingReactionData ComingReactionData, FComingAttackPayload ComingAttackPayload) const override;
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMovementAbilityData DodgeMovementAbilityData;
};
