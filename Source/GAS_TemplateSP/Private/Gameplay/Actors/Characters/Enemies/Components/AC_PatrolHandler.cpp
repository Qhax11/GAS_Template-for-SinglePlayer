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
		UE_LOG(LogTemp, Warning, TEXT("Enemy is null in: %s)"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemy->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyASC is null in %s, cannot initialize HeroControl."), *GetName());
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

void UAC_PatrolHandler::StopPatrolling()
{
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CancelMovementAbilities: ASC is null"));
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);

	OwnerEnemyASC->CancelAbilities(&CancelTags);
	bInPatrolling = false;

	if (!LastMoveToLocationAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_PatrolHandler::OnPatrollingAbilityEnded))
	{
		LastMoveToLocationAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UAC_PatrolHandler::OnPatrollingAbilityEnded);
	}
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

	FGameplayEventData MoveToLocationEventData;
	MoveToLocationEventData.EventTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Patrolling;
	MoveToLocationEventData.Target = PatrolPoints[CurrentIndex];

	UGAS_GameplayAbilityBase* PatrollingAbility =
		OwnerEnemyASC->TryActivateAbilityByClassWithEventData(EnemyPatrollingAbilityClass, MoveToLocationEventData);

	if (PatrollingAbility)
	{
		if (!PatrollingAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_PatrolHandler::OnPatrollingAbilityEnded))
		{
			PatrollingAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAC_PatrolHandler::OnPatrollingAbilityEnded);
		}
	}

	CurrentIndex++;
	LastMoveToLocationAbility = PatrollingAbility;
}

void UAC_PatrolHandler::OnPatrollingAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData)
{
	if (!bInPatrolling || !IsValid(this) || GetWorld()->bIsTearingDown)
	{
		return;
	}

	// Delay before moving to next point
	const float WaitTime = GetRandomPatrolWaitTime();

	FTimerHandle WaitTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &UAC_PatrolHandler::MoveToNextPatrolPoint, WaitTime, false);
}

float UAC_PatrolHandler::GetRandomPatrolWaitTime() const
{
	return FMath::RandRange(MinPatrolWaitTime, MaxPatrolWaitTime);
}
