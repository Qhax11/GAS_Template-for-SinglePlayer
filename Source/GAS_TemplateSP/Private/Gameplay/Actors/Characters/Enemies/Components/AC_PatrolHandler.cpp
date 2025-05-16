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
	if (PatrolPoints.Num() == 0)
	{
		return;
	}

	MoveToNextPatrolPoint();
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
}

void UAC_PatrolHandler::MoveToNextPatrolPoint()
{
	if (!OwnerEnemyASC)
	{
		return;
	} 

	if (!PatrolPoints.IsValidIndex(CurrentIndex)) 
	{
		return;
	}

	FGameplayEventData MoveToLocationEventData;
	MoveToLocationEventData.EventTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_MoveToLocation;
	MoveToLocationEventData.Target = PatrolPoints[CurrentIndex];

	UGAS_GameplayAbilityBase* MoveToLocationAbility = 
		OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MoveToLocationAbilityClass, MoveToLocationEventData);

	if (MoveToLocationAbility) 
	{
		if (!MoveToLocationAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_PatrolHandler::OnMoveToLocationAbilityEnded))
		{
			MoveToLocationAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAC_PatrolHandler::OnMoveToLocationAbilityEnded);
		}
	}

	CurrentIndex++;
	LastMoveToLocationAbility = MoveToLocationAbility;
}

void UAC_PatrolHandler::OnMoveToLocationAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData)
{
	if (!IsValid(this) || !GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	MoveToNextPatrolPoint();
}