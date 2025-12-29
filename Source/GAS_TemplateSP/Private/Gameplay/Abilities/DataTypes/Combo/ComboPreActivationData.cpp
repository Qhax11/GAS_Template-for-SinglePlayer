// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/DataTypes/Combo/ComboPreActivationData.h"

UComboPreActivationData* UComboPreActivationData::Initialize(FName InComboMontageSection,
	FGameplayTag InAdditionalTag, 
	FVector InMotionWarpingLocation, 
	FRotator InMotionWarpingRotation)
{
	ComboMontageSection = InComboMontageSection;
	AdditionalTag = InAdditionalTag;
	MotionWarpingLocation = InMotionWarpingLocation;
	MotionWarpingRotation = InMotionWarpingRotation;

	return this;
}
