// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyChaseTarget.h"

void UGA_EnemyChaseTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* TargetActor = EnemyController->GetTarget();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyController->MoveToActor(TargetActor, AcceptanceRadius, false, true, false, nullptr, true);
}
