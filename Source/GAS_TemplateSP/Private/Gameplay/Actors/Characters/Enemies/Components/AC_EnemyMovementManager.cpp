// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainData.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_EnemyMovementManager::UAC_EnemyMovementManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_EnemyMovementManager::BeginPlay()
{
	Super::BeginPlay();

	// Parent's check
	checkf(OwnerEnemy, TEXT("OwnerEnemy is null in %s"), *GetClass()->GetName());
	checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());
	checkf(OwnerEnemyASC, TEXT("OwnerEnemyASC is null in %s"), *GetClass()->GetName());

	AGAS_HeroBase* Hero = OwnerController->GetTargetHero();
	checkf(Hero, TEXT("Hero is null in %s"), *GetClass()->GetName());

	HeroMovementListener = Hero->GetMovementListenerComponent();
	checkf(HeroMovementListener, TEXT("HeroMovementListener is null in %s"), *GetClass()->GetName());
}

void UAC_EnemyMovementManager::ExecuteMovementChain(UMovementChainData* MovementChain)
{
	if (!MovementChain)
	{
		return;
	}

	// Start chain
	MovementChainTracker.StartChain(MovementChain);
	TryExecuteNextMovementAbilityInChain();
}

UGAS_GameplayAbilityBase* UAC_EnemyMovementManager::ExecuteReactionMovement(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload)
{
	if (!MovementData->IsValidData())
	{
		return nullptr;
	}

	// Reaction chain interrupt everything
	InterruptByReaction();

	// Activate with attack direction context
	UGAS_GameplayAbilityBase* ReactionAbility = ActivateMovementAbility(MovementData, AttackPayload);
	if (!ReactionAbility)
	{
		return nullptr;
	}

	ActivatedReactionAbility = ReactionAbility;
	return ReactionAbility;
}

UGAS_GameplayAbilityBase* UAC_EnemyMovementManager::ExecuteCorrectiveMovement(UMovementSingleData* MovementData)
{
	if (!MovementData->IsValidData())
	{
		return nullptr;
	}

	// Corrective movements don't need coming attack paylod
	return ActivateMovementAbility(MovementData);
}

void UAC_EnemyMovementManager::TryExecuteNextMovementAbilityInChain()
{
	UMovementSingleData* MovementDataInChain = MovementChainTracker.GetCurrentValidMovement();
	if (!MovementDataInChain)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: MovementDataInChain is null!"));
		return;
	}

	UGAS_GameplayAbilityBase* MovementAbilityInChain = ActivateMovementAbility(MovementDataInChain);
	if (MovementAbilityInChain)
	{
		// Bind end deleagte
		MovementAbilityInChain->OnAbilityEnded.RemoveAll(this);
		MovementAbilityInChain->OnAbilityEnded.AddUObject(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded);
		MovementChainTracker.CurrentMovementAbility = MovementAbilityInChain;
		UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_EnemyMovementManager: Movement Ability executed: %s"), *MovementAbilityInChain->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: Activation failed, skipping movement data."));
		MovementChainTracker.Advance();
		if (MovementChainTracker.IsChainFinished())
		{
			BroadcastChainEnd(EMovementChainResult::Aborted);
			ClearMovementChain();
			return;
		}
		TryExecuteNextMovementAbilityInChain();
	}
}

UGAS_GameplayAbilityBase* UAC_EnemyMovementManager::ActivateMovementAbility(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload)
{
	if (!MovementData->IsValidData())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: Validation failed!"));
		return nullptr;
	}

	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: OwnerEnemyASC is null!"));
		return nullptr;
	}

	ApplyDirectionPoliciesToMovementAbility(MovementData, AttackPayload);

	// TODO: USE ONLY JUST EVENT TAG AND OPTIONAL OBJECT?
	FGameplayEventData EventData;
	EventData.EventTag = MovementData->AbilityTriggerTag;
	EventData.OptionalObject = MovementData;
	EventData.InstigatorTags.AddTag(MovementData->DirectionTag);
	EventData.EventMagnitude = MovementData->ExpectedDuration;

	return OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementData->MovementAbilityClass, EventData);
}

void UAC_EnemyMovementManager::ApplyDirectionPoliciesToMovementAbility(UMovementSingleData* MovementAbilityData, const FComingAttackPayload& AttackPayload)
{
	if (!MovementAbilityData)
	{
		return;
	}

	// Policy kapalı → sadece garanti ver
	if (!MovementAbilityData->EnableDirectionPolicy || !MovementAbilityData->DirectionPolicyTag.IsValid())
	{
		if (!MovementAbilityData->DirectionTag.IsValid())
		{
			MovementAbilityData->DirectionTag = GetRandomRepositionDirectionTag();
		}
		return;
	}

	// Policy açık → resolve et ve ata
	MovementAbilityData->DirectionTag = ResolveDirectionFromPolicy(MovementAbilityData->DirectionPolicyTag, AttackPayload);
}

FGameplayTag UAC_EnemyMovementManager::ResolveDirectionFromPolicy(const FGameplayTag& PolicyTag, const FComingAttackPayload& AttackPayload)
{
	FGameplayTag ResolvedTag;

	if (PolicyTag == GAS_Tags::TAG_AI_Direction_Policy_EscapeFromAttack)
	{
		ResolvedTag = ResolveAttackDirection(AttackPayload.AttackDirectionTag);
	}
	else if (PolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
	{
		if (HeroMovementListener)
		{
			FGameplayTag LastDirection = HeroMovementListener->GetHeroLastMovementDirectionTagByLastInput();
			if (LastDirection.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: LastDirection is: %s"), *LastDirection.ToString());
				if(LastDirection == GAS_Tags::TAG_Gameplay_Direction_Forward || LastDirection == GAS_Tags::TAG_Gameplay_Direction_Backward)
				{
					// If the last direction is forward, we want to avoid moving forward as an enemy.
					ResolvedTag = GetRandomStrafeDirectionTag();
				}
				else
				{
					ResolvedTag = LastDirection;
				}
			}
		}
	}
	else if (PolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
	{
		ResolvedTag = GetRandomRepositionDirectionTag();
	}

	// Fallback guarantee
	if (!ResolvedTag.IsValid())
	{
		ResolvedTag = GetRandomRepositionDirectionTag();
	}

	return ResolvedTag;
}

FGameplayTag UAC_EnemyMovementManager::GetRandomStrafeDirectionTag()
{
	static const TArray<FGameplayTag> PossibleDirections =
	{
		GAS_Tags::TAG_Gameplay_Direction_Left,
		GAS_Tags::TAG_Gameplay_Direction_Right
	};

	int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
	return PossibleDirections[RandomIndex];
}

FGameplayTag UAC_EnemyMovementManager::GetRandomRepositionDirectionTag()
{
	static const TArray<FGameplayTag> PossibleDirections =
	{
		GAS_Tags::TAG_Gameplay_Direction_Forward,
		GAS_Tags::TAG_Gameplay_Direction_Backward,
		GAS_Tags::TAG_Gameplay_Direction_Left,
		GAS_Tags::TAG_Gameplay_Direction_Right
	};

	int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
	return PossibleDirections[RandomIndex];
}

FGameplayTag UAC_EnemyMovementManager::ResolveAttackDirection(FGameplayTag AttackDirectionTag)
{
	if (!AttackDirectionTag.IsValid())
	{
		return FGameplayTag();
	}

	if (AttackDirectionTag == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_RightToLeft)
	{
		return GAS_Tags::TAG_Gameplay_Direction_Right;
	}
	else if (AttackDirectionTag == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_LeftToRight)
	{
		return GAS_Tags::TAG_Gameplay_Direction_Left;
	}
	else if (AttackDirectionTag == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_BottomToTop)
	{
		return GAS_Tags::TAG_Gameplay_Direction_Backward;
	}
	else if (AttackDirectionTag == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_TopToBottom)
	{
		return GAS_Tags::TAG_Gameplay_Direction_Backward;
	}

	return FGameplayTag();
}

void UAC_EnemyMovementManager::OnMovementAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.AbilityThatEnded != MovementChainTracker.CurrentMovementAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: Ended movement ability is not CurrentMovement"), *AbilityEndedData.AbilityThatEnded->GetName());
		return;
	}

	MovementChainTracker.CurrentMovementAbility = nullptr;

	if (AbilityEndedData.bWasCancelled)
	{
		FMovementChainEndData MovementChainEndData;

		UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_EnemyMovementManager: Chain cancelled by %s. Resetting."), *AbilityEndedData.AbilityThatEnded->GetName());
		// IMPORTANT: clear first, then broadcast
		BroadcastChainEnd(EMovementChainResult::Aborted);
		ClearMovementChain();
		return;
	}

	MovementChainTracker.Advance();
	if (MovementChainTracker.IsChainFinished())
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_EnemyMovementManager: Chain Completed."));
		// IMPORTANT: clear first, then broadcast
		BroadcastChainEnd(EMovementChainResult::Completed);
		ClearMovementChain();
		return;
	}
	else
	{
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::BroadcastChainEnd(EMovementChainResult Result)
{
	FMovementChainEndData MovementChainEndData;
	MovementChainEndData.ChainData = MovementChainTracker.CuurentChainData;
	MovementChainEndData.Result = Result;
	OnMovementChainEnded.Broadcast(MovementChainEndData);
}

void UAC_EnemyMovementManager::ClearMovementChain()
{
	if (MovementChainTracker.CurrentMovementAbility)
	{
		MovementChainTracker.CurrentMovementAbility->OnAbilityEnded.RemoveAll(this);
	}

	MovementChainTracker.CurrentMovementAbility = nullptr;
	MovementChainTracker.ResetChain();
}

void UAC_EnemyMovementManager::InterruptByReaction()
{
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: OwnerEnemyASC is null"));
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);

	OwnerEnemyASC->CancelAbilities(&CancelTags);
}
