// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"
#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"

UGA_EnemyMovementBase::UGA_EnemyMovementBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);
}

void UGA_EnemyMovementBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EnemyCharacter = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyCharacter is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyMovementComp = EnemyCharacter->GetCharacterMovement();
	if (!EnemyMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyMovementComp is null in: %s!, Ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyController = Cast<AAIControllerBase>(EnemyCharacter->GetController());
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyController is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	//============ ** ** ============//
	EnemyMovementComp->MaxWalkSpeed = MovementSpeed;
	CachedExpectedDuration = TriggerEventData->EventMagnitude;
}

void UGA_EnemyMovementBase::ExecuteMoveTask(UAT_AIMoveTo* MoveTask)
{
	if (!MoveTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: MoveTask is null in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	MoveTask->SetExpectedDuration(CachedExpectedDuration);

	MoveTask->OnCompleted.AddDynamic(this, &UGA_EnemyMovementBase::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this, &UGA_EnemyMovementBase::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this, &UGA_EnemyMovementBase::OnMoveFailed);
	MoveTask->ExpectedDurationReached.AddDynamic(this, &UGA_EnemyMovementBase::OnExpectedDurationFinished);
	MoveTask->MinDurationReached.AddDynamic(this, &UGA_EnemyMovementBase::OnMinDurationFinished);
	MoveTask->MaxDurationReached.AddDynamic(this, &UGA_EnemyMovementBase::HandleMaxDurationReached);

	MoveTask->ReadyForActivation();
}

void UGA_EnemyMovementBase::OnMoveCompleted()
{
	UE_LOG(LogTemp, Log, TEXT("Ability: UGA_EnemyMovementBase: Moving is Completed, Ability ended"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyMovementBase::OnMoveAborted()
{
	UE_LOG(LogTemp, Log, TEXT("Ability: UGA_EnemyMovementBase: Moving is Aborted, Ability ended with canceling"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_EnemyMovementBase::OnMoveFailed()
{
	UE_LOG(LogTemp, Log, TEXT("Ability: UGA_EnemyMovementBase: Moving is Failed, ability ended"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyMovementBase::HandleMaxDurationReached()
{
	UE_LOG(LogTemp, Log, TEXT("Ability: UGA_EnemyMovementBase: MaxDuration is Reached, ability ended"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyMovementBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

