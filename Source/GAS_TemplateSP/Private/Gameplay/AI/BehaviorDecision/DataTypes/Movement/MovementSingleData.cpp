// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"

bool UMovementSingleData::IsValidData() const
{
	if (bDisableMovement) 
	{
		return false;
	}

	if (!MovementAbilityClass || !AbilityTriggerTag.IsValid())
	{
		return false;
	}

	return true;
}
