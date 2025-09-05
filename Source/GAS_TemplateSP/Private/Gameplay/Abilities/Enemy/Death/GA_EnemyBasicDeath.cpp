// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyBasicDeath.h"

UGA_EnemyBasicDeath::UGA_EnemyBasicDeath()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_Death_Basic;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
}

void UGA_EnemyBasicDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DisableOwnerCollision(ECollisionEnabled::NoCollision);
}
