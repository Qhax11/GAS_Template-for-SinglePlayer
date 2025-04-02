// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"

UGA_EnemyMovementBase::UGA_EnemyMovementBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
		UE_LOG(LogTemp, Warning, TEXT("BossCharacter is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyMovementComp = EnemyCharacter->GetCharacterMovement();
	if (!EnemyMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyMovementComp is null in: %s!, Ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyController = Cast<AAIControllerBase>(EnemyCharacter->GetController());
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossController is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyMovementComp->MaxWalkSpeed = MovementSpeed;
}

void UGA_EnemyMovementBase::RequestMoveToLocation(const FVector& MoveLocation)
{
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalLocation(MoveLocation);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);

	FNavPathSharedPtr NavPath;
	EnemyController->MoveTo(MoveReq, &NavPath);

	MoveCompleteHandle = EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
		this, &ThisClass::OnMoveCompleted);
}

void UGA_EnemyMovementBase::RequestMoveToTarget(AActor* TargetActor)
{
	if (!EnemyController || !TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestMoveToTarget failed in %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(TargetActor); 
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);

	FNavPathSharedPtr NavPath;
	EnemyController->MoveTo(MoveReq, &NavPath);

	MoveCompleteHandle = EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
		this, &UGA_EnemyMovementBase::OnMoveCompleted);
}

void UGA_EnemyMovementBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveTo strafing failed or was aborted: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	if (EnemyController && EnemyController->GetPathFollowingComponent())
	{
		EnemyController->GetPathFollowingComponent()->OnRequestFinished.Remove(MoveCompleteHandle);
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyMovementBase::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

