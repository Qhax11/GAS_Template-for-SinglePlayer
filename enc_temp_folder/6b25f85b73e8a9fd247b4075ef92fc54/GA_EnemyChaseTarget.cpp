// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyChaseTarget.h"

UGA_EnemyChaseTarget::UGA_EnemyChaseTarget()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_ChaseTarget;

	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyChaseTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AActor* TargetActor = EnemyController->GetTargetActor();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (TriggerEventData->EventMagnitude > 0) 
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				MovementTimerHandle,
				this,
				&UGA_EnemyChaseTarget::OnChaseTimeEnd,
				TriggerEventData->EventMagnitude,
				false
			);
		}
	}

	RequestMoveToTarget(TargetActor);
}

void UGA_EnemyChaseTarget::OnChaseTimeEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
