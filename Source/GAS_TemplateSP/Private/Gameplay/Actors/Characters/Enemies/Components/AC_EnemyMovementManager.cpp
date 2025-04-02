// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"

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
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityMovementChainSet, AbilityClass or OwnerEnemyASC is null in: %s !"), *GetName());
		return;
	}

	const TArray<FMovementChainData>* MovementChain = GetMovementChainForAbility(AbilityClass);
	if (MovementChain && MovementChain->Num() > 0)
	{
		ActiveMovementChain = *MovementChain;
		CurrentMovementChainIndex = 0;

		TryExecuteNextMovementAbilityInChain();
	}
}

const TArray<FMovementChainData>* UAC_EnemyMovementManager::GetMovementChainForAbility(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass) const
{
	for (const FAbilityMovementChain& Mapping : AbilityMovementChainSet->ChainMappings)
	{
		if (Mapping.Ability == AbilityClass)
		{
			if (Mapping.MovementChains.Num() > 0 && Mapping.MovementChains[0])
			{
				return &Mapping.MovementChains[0]->Movements; // Þimdilik sadece ilk MovementChain'i alýyoruz
			}
		}
	}

	return nullptr;
}

void UAC_EnemyMovementManager::TryExecuteNextMovementAbilityInChain()
{
	if (!ActiveMovementChain.IsValidIndex(CurrentMovementChainIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Movement chain finished on %s"), *GetName());
		return;
	}

	const FMovementChainData& MovementData = ActiveMovementChain[CurrentMovementChainIndex];
	TryActivateMovementAbilityWithEventData(MovementData);
}

void UAC_EnemyMovementManager::TryActivateMovementAbilityWithEventData(FMovementChainData MovementChainData)
{
	if (!MovementChainData.MovementAbilityClass || !MovementChainData.TriggerTag.IsValid()) 
	{
		UE_LOG(LogTemp, Log, TEXT("MovementAbilityClass or TriggerTag is null in: %s!"), *GetName());
	}

	FGameplayEventData MovementAbilityEventData;
	MovementAbilityEventData.EventTag = MovementChainData.TriggerTag;
	MovementAbilityEventData.InstigatorTags.AddTag(MovementChainData.DirectionTag);
	UGAS_GameplayAbilityBase* MovementAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementChainData.MovementAbilityClass, MovementAbilityEventData);
	if (MovementAbility) 
	{
		UGAS_GameplayAbilityBase* CDO = MovementChainData.MovementAbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();

		if (!CDO->OnGameplayAbilityEndedWithData.IsAlreadyBound(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded))
		{
			CDO->OnGameplayAbilityEndedWithData.AddDynamic(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded);
		}
	}
	else
	{
		// Ability aktive edilemediyse bile zinciri sürdür
		CurrentMovementChainIndex++;
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::OnMovementAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.bWasCancelled)
	{
		// Oyundan çýkarken veya ability iptal edilince zinciri devam ettirme
		return;
	}

	CurrentMovementChainIndex++;
	TryExecuteNextMovementAbilityInChain();
}



