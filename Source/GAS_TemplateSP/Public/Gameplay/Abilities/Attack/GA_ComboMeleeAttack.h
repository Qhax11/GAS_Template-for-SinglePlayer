// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "GA_ComboMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_ComboMeleeAttack : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

protected:

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;


};
