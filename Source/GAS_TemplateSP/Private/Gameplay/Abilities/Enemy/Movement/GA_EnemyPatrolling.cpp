// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyPatrolling.h"
#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"

UGA_EnemyPatrolling::UGA_EnemyPatrolling()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Patrolling;

	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyPatrolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyPatrolling: TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	FVector TargetLocation = TriggerEventData->Target->GetActorLocation();

	// TEK satýr! Her þey task içinde halloluyor
	UAT_AIMoveTo* MoveTask = UAT_AIMoveTo::AIMoveTo(
		this,
		FName("StrafeMove"),
		EnemyController,
		TargetLocation,
		AcceptanceRadius,
		5,  // Min duration support built-in!
		MovementSpeed
	);

	MoveTask->OnCompleted.AddDynamic(this, &UGA_EnemyPatrolling::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this, &UGA_EnemyPatrolling::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this, &UGA_EnemyPatrolling::OnMoveFailed);
	MoveTask->ReadyForActivation();
}

void UGA_EnemyPatrolling::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyPatrolling::OnMoveAborted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_EnemyPatrolling::OnMoveFailed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

