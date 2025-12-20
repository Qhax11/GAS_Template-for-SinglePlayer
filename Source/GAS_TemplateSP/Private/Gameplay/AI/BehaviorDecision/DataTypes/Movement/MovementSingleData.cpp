// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"

bool UMovementSingleData::IsValidData() const
{
	if (!MovementAbilityClass || !AbilityTriggerTag.IsValid())
	{
		return false;
	}

	if (EnableDirectionPolicy && !DirectionPolicyTag.IsValid())
	{
		return false;
	}

	return true;
}
