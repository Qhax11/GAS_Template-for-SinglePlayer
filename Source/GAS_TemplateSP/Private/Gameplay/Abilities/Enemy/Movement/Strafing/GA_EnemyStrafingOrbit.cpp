// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/Strafing/GA_EnemyStrafingOrbit.h"

UGA_EnemyStrafingOrbit::UGA_EnemyStrafingOrbit()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Strafing_Orbit;

	AbilityTriggers.Add(TriggerData);
}
