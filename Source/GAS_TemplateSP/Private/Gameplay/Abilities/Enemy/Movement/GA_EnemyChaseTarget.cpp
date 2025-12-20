// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyChaseTarget.h"
#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"

UGA_EnemyChaseTarget::UGA_EnemyChaseTarget()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_ChaseTarget;

	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Rotation_LockTowardsTarget);
}

void UGA_EnemyChaseTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyChaseTarget: TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AActor* TargetActor = EnemyController->GetTargetActor();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyChaseTarget: TargetActor is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UAT_AIMoveTo* MoveTask = UAT_AIMoveTo::AIMoveToActor(
		this,
		FName("Chase"),
		EnemyController,
		TargetActor,
		AcceptanceRadius,
		MinMovementDuration,
		MaxMovementDuration,
		MovementSpeed
	);

	ExecuteMoveTask(MoveTask);
}

void UGA_EnemyChaseTarget::OnExpectedDurationFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyChaseTarget::OnMinDurationFinished()
{
}



