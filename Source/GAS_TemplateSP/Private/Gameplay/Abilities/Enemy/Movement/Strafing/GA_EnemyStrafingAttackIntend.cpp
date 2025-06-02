// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/Strafing/GA_EnemyStrafingAttackIntend.h"

UGA_EnemyStrafingAttackIntend::UGA_EnemyStrafingAttackIntend()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Strafing_AttackIntend;

	AbilityTriggers.Add(TriggerData);
}
