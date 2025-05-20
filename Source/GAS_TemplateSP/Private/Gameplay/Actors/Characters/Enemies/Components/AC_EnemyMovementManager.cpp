// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_EnemyMovementManager::UAC_EnemyMovementManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_EnemyMovementManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemyBase = Cast<AGAS_EnemyBase>(GetOwner());
	if (!OwnerEnemyBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyBase is null in: %s !"), *GetName());
		return;
	}

	OwnerController = Cast<AAIControllerBase>(OwnerEnemyBase->GetController());
	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
		return;
	}

	BehaviorDecisionComp = OwnerController->GetBehaviorDecisionComponent();
	if (!BehaviorDecisionComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComp is null in: %s !"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemyBase->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyASC is null in: %s !"), *GetName());
		return;
	}
}

void UAC_EnemyMovementManager::StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAbilityClass)
{
	if (!BehaviorDecisionComp || !SelectedAbilityClass || !OwnerEnemyASC) 
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComp, SelectedAbilityClass or OwnerEnemyASC is null in: %s!"), *GetName());
		return;
	}

	if (MovementChainTracker.bIsActive)
	{
		/*
		if (MovementChainTracker.IsCurrentAbilityStillValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Chain already active and current ability still valid. Skipping start."));
			return;
		}
		*/
		UE_LOG(LogTemp, Warning, TEXT("Chain is marked active but ability is invalid. Restarting chain."));
		MovementChainTracker.ResetChain();
	}

	const TArray<FMovementAbilityData> MovementData = BehaviorDecisionComp->GetBestMovementChain(SelectedAbilityClass);
	if (MovementData.Num() > 0)
	{
		MovementChainTracker.StartChain(MovementData);
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::StopMovementAbilities()
{
	CancelMovementAbilities();
	MovementChainTracker.ResetChain();
}

void UAC_EnemyMovementManager::CancelMovementAbilities()
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

void UAC_EnemyMovementManager::TryExecuteNextMovementAbilityInChain()
{
	if (MovementChainTracker.IsChainFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("Chain finished."));
		StopMovementAbilities();
		OnMovementChainEnded.Broadcast();
		return;
	}

	if (const FMovementAbilityData* Data = MovementChainTracker.GetCurrentMovementAbilityInChain())
	{
		TryActivateMovementAbilityWithEventData(*Data);
	}
}

void UAC_EnemyMovementManager::TryActivateMovementAbilityWithEventData(FMovementAbilityData MovementChainData)
{
	if (!MovementChainData.MovementAbilityClass || !MovementChainData.AbilityTriggerTag.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("MovementAbilityClass or TriggerTag is null in: %s!"), *GetName());
	}

	FGameplayEventData MovementAbilityEventData;
	MovementAbilityEventData.EventTag = MovementChainData.AbilityTriggerTag;
	MovementAbilityEventData.InstigatorTags.AddTag(MovementChainData.ResolvedDirectionTag);
	MovementAbilityEventData.EventMagnitude = MovementChainData.TimeLimit;

	UGAS_GameplayAbilityBase* MovementAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementChainData.MovementAbilityClass, MovementAbilityEventData);
	if (MovementAbility) 
	{
		MovementChainTracker.CurrentMovementAbility = MovementAbility;
		if (!MovementAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded))
		{
			MovementAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded);
		}
	}
	else
	{
		MovementChainTracker.Advance();
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::OnMovementAbilityEnded(const FAbilityEndedDataBP& AbilityEndedData)
{
	if (AbilityEndedData.AbilityThatEnded != MovementChainTracker.CurrentMovementAbility)
	{
		return;
	}

	MovementChainTracker.CurrentMovementAbility = nullptr;

	if (AbilityEndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Log, TEXT("Chain cancelled by %s. Resetting."), *AbilityEndedData.AbilityThatEnded->GetName());
		StopMovementAbilities();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Movement Ability ended: %s."), *AbilityEndedData.AbilityThatEnded->GetName());
	MovementChainTracker.Advance();
	TryExecuteNextMovementAbilityInChain();
}



