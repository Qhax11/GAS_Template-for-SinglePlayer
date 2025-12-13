// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "ComingAttackReactionDataDodge.generated.h"

class UAC_HeroMovementListener;

UCLASS()
class GAS_TEMPLATESP_API UComingAttackReactionDataDodge : public UComingAttackReactionData
{
	GENERATED_BODY()
	
public:
	UComingAttackReactionDataDodge(); 

	virtual bool IsEnable(UComingAttackReactionData* ComingReactionData, FComingAttackPayload ComingAttackPayload) const override;

	void ApplyDirectionPoliciesToMovementAbility(UAC_HeroMovementListener* HeroMovementListener);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMovementAbilityData DodgeMovementAbilityData;

private:
	FGameplayTag GetRandomDirectionTag();
};
