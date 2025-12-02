// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "BDS_ComingAttackReaction_Dodge.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UBDS_ComingAttackReaction_Dodge : public UComingAttackReactionData
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
