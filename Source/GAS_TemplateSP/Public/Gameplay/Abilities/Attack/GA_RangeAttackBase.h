// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_RangeAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_RangeAttackBase : public UGA_MontageAbility
{
	GENERATED_BODY()

protected:

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;
	
};
