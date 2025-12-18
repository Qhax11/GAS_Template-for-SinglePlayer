// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossTargetBase.h"

UGA_BossTargetBase::UGA_BossTargetBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BossTargetBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	BossCharacter = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo());
	if (!BossCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_BossTargetBase: BossCharacter is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	BossController = Cast<AAIControllerBase>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	if (!BossController) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_BossTargetBase: BossController is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (bActorWillSpawnWithEQS)
	{
		StartEQSForTargetActorSpawnLocation();
	}
	else
	{
		SpawnAndSetupTargetActor();
	}
}

