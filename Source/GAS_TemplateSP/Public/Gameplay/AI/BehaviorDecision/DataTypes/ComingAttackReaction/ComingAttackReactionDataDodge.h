// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "ComingAttackReactionDataDodge.generated.h"

class UAC_HeroMovementListener;

UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataDodge : public UComingAttackReactionData
{
	GENERATED_BODY()
	
public:
	UComingAttackReactionDataDodge(); 

	virtual bool IsEnable(FComingAttackPayload ComingAttackPayload, FReactionEnableDebug* OutDebug = nullptr) const override;

	bool PassesChanceRoll(const UAbilitySystemComponent* ASC, FReactionChanceDebug* OutDebug) const override;

	// What movement ability data to use for dodge reaction
	UPROPERTY(EditDefaultsOnly, Instanced)
	UMovementSingleData* DodgeMovementAbilityData;
};
