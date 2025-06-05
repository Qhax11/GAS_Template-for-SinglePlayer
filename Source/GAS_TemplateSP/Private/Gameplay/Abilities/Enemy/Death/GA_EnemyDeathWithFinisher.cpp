// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathWithFinisher.h"

UGA_EnemyDeathWithFinisher::UGA_EnemyDeathWithFinisher()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_FinisherDeath;

	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyDeathWithFinisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyDeathWithFinisher ActivateAbility"));
	//DisableOwnerCollision();
}

void UGA_EnemyDeathWithFinisher::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	DisableOwnerCollision();
}
