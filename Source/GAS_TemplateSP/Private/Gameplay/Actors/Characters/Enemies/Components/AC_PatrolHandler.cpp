// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_PatrolHandler.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"

UAC_PatrolHandler::UAC_PatrolHandler()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_PatrolHandler::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemy = Cast<AGAS_EnemyBase>(GetOwner());
	if (!OwnerEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PatrolHandler: Enemy is null in: %s)"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemy->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PatrolHandler: EnemyASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}
}

void UAC_PatrolHandler::StartPatrolling()
{
	if (PatrolPoints.Num() == 0 || !OwnerEnemyASC)
	{
		return;
	}

	MoveToNextPatrolPoint();
	bInPatrolling = true;
}

void UAC_PatrolHandler::MoveToNextPatrolPoint()
{
	if (!PatrolPoints.IsValidIndex(CurrentIndex))
	{
		CurrentIndex = 0;
	}

	ActivatePatrollingAbility();
}

void UAC_PatrolHandler::ActivatePatrollingAbility()
{
	if (!OwnerEnemyASC)
	{
		return;
	}

	if (!PatrolPoints.IsValidIndex(CurrentIndex))
	{
		CurrentIndex = 0;
	}

	AActor* PatrolTarget = PatrolPoints[CurrentIndex];
	if (!IsValid(PatrolTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PatrolHandler: Invalid patrol point at index %d on %s"), CurrentIndex, *GetName());
		CurrentIndex++;
		return; // ability'ye gitme
	}

	FGameplayEventData MoveToLocationEventData;
	MoveToLocationEventData.EventTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Patrolling;
	MoveToLocationEventData.Target = PatrolPoints[CurrentIndex];

	UGAS_GameplayAbilityBase* PatrollingAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(EnemyPatrollingAbilityClass, MoveToLocationEventData);
	if (!PatrollingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PatrolHandler: PatrollingAbility is null in %s"), *GetName());
		return;
	}

	PatrollingAbility->OnAbilityEnded.RemoveAll(this);
	PatrollingAbility->OnAbilityEnded.AddUObject(this, &UAC_PatrolHandler::OnPatrollingAbilityEnded);
	LastPatrolingAbility = PatrollingAbility;
	CurrentIndex++;
}

void UAC_PatrolHandler::StopPatrolling()
{
	if (!bInPatrolling)
	{
		return;
	}

	bInPatrolling = false;

	if (LastPatrolingAbility)
	{
		LastPatrolingAbility->OnAbilityEnded.RemoveAll(this);
		LastPatrolingAbility->EndAbilityManually();
		LastPatrolingAbility = nullptr;
	}

	GetWorld()->GetTimerManager().ClearTimer(WaitForNextPatrolTimerHandle);

	OnPatrollingStopped.Broadcast();
}

void UAC_PatrolHandler::OnPatrollingAbilityEnded(const FCustomAbilityEndedData& ShadowAttackAbilityEndedData)
{
	if (!bInPatrolling || !IsValid(this) || GetWorld()->bIsTearingDown)
	{
		return;
	}

	// Delay before moving to next point
	const float WaitTime = GetRandomPatrolWaitTime();
	GetWorld()->GetTimerManager().SetTimer(WaitForNextPatrolTimerHandle, this, &UAC_PatrolHandler::MoveToNextPatrolPoint, WaitTime, false);
}

float UAC_PatrolHandler::GetRandomPatrolWaitTime() const
{
	return FMath::RandRange(MinPatrolWaitTime, MaxPatrolWaitTime);
}

void UAC_PatrolHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(WaitForNextPatrolTimerHandle);
	Super::EndPlay(EndPlayReason);
}