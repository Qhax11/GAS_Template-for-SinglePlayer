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

	// Actor'a doðru chase - min duration 0 (hemen bitebilir)
	UAT_AIMoveTo* MoveTask = UAT_AIMoveTo::AIMoveToActor(
		this,
		FName("Chase"),
		EnemyController,
		TargetActor,
		AcceptanceRadius,
		0.0f,  // No min duration for chase
		MovementSpeed
	);

	MoveTask->OnCompleted.AddDynamic(this, &UGA_EnemyChaseTarget::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this, &UGA_EnemyChaseTarget::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this, &UGA_EnemyChaseTarget::OnMoveFailed);
	MoveTask->ReadyForActivation();
}

void UGA_EnemyChaseTarget::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyChaseTarget::OnMoveAborted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_EnemyChaseTarget::OnMoveFailed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}


