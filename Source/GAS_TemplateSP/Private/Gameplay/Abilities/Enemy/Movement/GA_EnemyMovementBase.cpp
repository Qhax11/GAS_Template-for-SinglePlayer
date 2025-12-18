// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"

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

	EnemyMovementComp->MaxWalkSpeed = MovementSpeed;
}

void UGA_EnemyMovementBase::RequestMoveToLocation(const FVector& MoveLocation)
{
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyController is null in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: Move Location: %s, from: %s"), *MoveLocation.ToString(), *GetName());

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalLocation(MoveLocation);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);

	FNavPathSharedPtr NavPath;
	EPathFollowingRequestResult::Type MoveResult = EnemyController->MoveTo(MoveReq, &NavPath);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: MoveTo failed immediately in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	UPathFollowingComponent* PathComp = EnemyController->GetPathFollowingComponent();
	if (!PathComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: PathFollowingComponent is null in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	PathComp->OnRequestFinished.AddUObject(this, &ThisClass::OnMoveCompleted);
}

void UGA_EnemyMovementBase::RequestMoveToTarget(AActor* TargetActor)
{
	if (!EnemyController || !TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: RequestMoveToTarget failed in %s"), *GetName());
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

	EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
		this, &UGA_EnemyMovementBase::OnMoveCompleted);
}

void UGA_EnemyMovementBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (bAbilityEnded) 
	{
		return;
	}

	if (!Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: MoveTo strafing failed or was aborted: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyMovementBase::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	bAbilityEnded = true;

	if (EnemyController)
	{
		EnemyController->StopMovement(); 
	}

	if (EnemyController && EnemyController->GetPathFollowingComponent())
	{
		EnemyController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(MovementTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	}

	// Super::EndAbility must be called last because it broadcasts the end event immediately.
	// Calling it early may trigger delegates or cleanup logic before this ability finishes its own cleanup.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

