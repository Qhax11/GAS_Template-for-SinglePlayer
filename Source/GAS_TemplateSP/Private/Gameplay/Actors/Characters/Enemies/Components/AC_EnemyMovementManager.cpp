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

void UAC_EnemyMovementManager::StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	if (!AbilityMovementChainSet || !AbilityClass || !OwnerEnemyASC)
		return;

	if (MovementChainTracker.bIsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chain already active, skipping start."));
		return;
	}

	BehaviorDecisionComp->GetBestMovementChain()
	const TArray<FMovementAbilityData>* MovementData = GetMovementChainForAbility(AbilityClass);
	if (MovementData && MovementData->Num() > 0)
	{
		MovementChainTracker.StartChain(*MovementData);
		TryExecuteNextMovementAbilityInChain();
	}
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

const TArray<FMovementAbilityData>* UAC_EnemyMovementManager::GetMovementChainForAbility(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass) const
{
	for (const FAttackAbilityMovementChain& Mapping : AbilityMovementChainSet->ChainMappings)
	{
		if (Mapping.AttackAbilityClass == AbilityClass)
		{
			if (Mapping.MovementChains.Num() > 0 && Mapping.MovementChains[0])
			{
				return &Mapping.MovementChains[0]->MovementChain; // Þimdilik sadece ilk MovementChain'i alýyoruz
			}
		}
	}

	return nullptr;
}

void UAC_EnemyMovementManager::TryExecuteNextMovementAbilityInChain()
{
	if (MovementChainTracker.IsChainFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("Chain finished."));
		MovementChainTracker.ResetChain();
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
	MovementAbilityEventData.InstigatorTags.AddTag(MovementChainData.DirectionTag);
	UGAS_GameplayAbilityBase* MovementAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementChainData.MovementAbilityClass, MovementAbilityEventData);
	if (MovementAbility) 
	{
		if (!MovementAbility->OnGameplayAbilityEndedWithData.IsAlreadyBound(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded))
		{
			MovementAbility->OnGameplayAbilityEndedWithData.AddDynamic(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded);
		}
	}
	else
	{
		// Ability aktive edilemediyse bile zinciri sürdür
		MovementChainTracker.Advance();
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::OnMovementAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Log, TEXT("Chain cancelled by ability. Resetting."));
		MovementChainTracker.ResetChain();
		OnMovementChainEnded.Broadcast();
		return;
	}

	MovementChainTracker.Advance();
	TryExecuteNextMovementAbilityInChain();
}



