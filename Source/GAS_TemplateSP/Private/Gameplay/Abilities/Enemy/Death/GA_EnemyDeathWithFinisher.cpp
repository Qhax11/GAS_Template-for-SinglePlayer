// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathWithFinisher.h"

UGA_EnemyDeathWithFinisher::UGA_EnemyDeathWithFinisher()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_DeadWithFinisher);
}

void UGA_EnemyDeathWithFinisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//DisableOwnerCollision();
}
