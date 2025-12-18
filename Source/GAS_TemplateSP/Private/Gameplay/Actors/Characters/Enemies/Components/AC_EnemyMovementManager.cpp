// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementChainDataa.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
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

void UAC_EnemyMovementManager::ExecuteMovementChain(UMovementChainDataa* MovementChain)
{
	if (!MovementChain || !OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovementChainData or OwnerEnemyASC is null in: %s!"), *GetName());
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

	if (MovementChain->MovementChain.Num() > 0)
	{
		MovementChainTracker.StartChain(MovementChain->MovementChain);
		TryExecuteNextMovementAbilityInChain();
	}
}

bool UAC_EnemyMovementManager::ExecuteReactionMovement(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload)
{
	if (!MovementData || !OwnerEnemyASC)
	{
		return false;
	}

	if (!MovementData->AbilityTriggerTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_EnemyMovementManager: AbilityTriggerTag is non valid!"));
		return false;
	}

	TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass = MovementData->MovementAbilityClass;
	if (!MovementAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_EnemyMovementManager: MovementAbilityClass is non valid!"));
		return false;
	}
	
	ApplyDirectionPoliciesToMovementAbility(MovementData, AttackPayload.AttackDirectionTag);

	StopMovementAbilities();

	FGameplayEventData GameplayEventData = FGameplayEventData();
	GameplayEventData.InstigatorTags.AddTag(MovementData->ResolvedDirectionTag);
	GameplayEventData.EventTag = MovementData->AbilityTriggerTag;
	GameplayEventData.EventMagnitude = MovementData->AbilityEventMagnitude;

	UGAS_GameplayAbilityBase* ReactionMovementAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementAbilityClass, GameplayEventData);
	if (!ReactionMovementAbility)
	{
		return false;
	}

	ReactionMovementAbility->OnAbilityEnded.RemoveAll(this);
	ReactionMovementAbility->OnAbilityEnded.AddUObject(this, &UAC_EnemyMovementManager::OnReactionMovementAbilityEnded);
	ActivatedReactionAbility = ReactionMovementAbility;

	return true;
}

void UAC_EnemyMovementManager::OnReactionMovementAbilityEnded(const FCustomAbilityEndedData& EndData)
{
	FReactionMovementEndedData Result;
	Result.bWasCancelled = EndData.bWasCancelled;

	OnReactionMovementEnded.Broadcast(Result);
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

	if (UMovementSingleData* Data = MovementChainTracker.GetCurrentMovementAbilityInChain())
	{
		TryActivateMovementAbilityWithEventData(Data);
	}
}

void UAC_EnemyMovementManager::TryActivateMovementAbilityWithEventData(UMovementSingleData* MovementChainData)
{
	if (!MovementChainData->MovementAbilityClass || !MovementChainData->AbilityTriggerTag.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("MovementAbilityClass or TriggerTag is null in: %s!"), *GetName());
		return;
	}

	// Before the TryActivate
	ApplyDirectionPoliciesToMovementAbility(MovementChainData);

	FGameplayEventData MovementAbilityEventData;
	MovementAbilityEventData.EventTag = MovementChainData->AbilityTriggerTag;
	MovementAbilityEventData.InstigatorTags.AddTag(MovementChainData->ResolvedDirectionTag);
	MovementAbilityEventData.EventMagnitude = MovementChainData->AbilityEventMagnitude;

	UGAS_GameplayAbilityBase* MovementAbility = OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementChainData->MovementAbilityClass, MovementAbilityEventData);
	if (MovementAbility) 
	{
		MovementChainTracker.CurrentMovementAbility = MovementAbility;

		MovementAbility->OnAbilityEnded.RemoveAll(this);
		MovementAbility->OnAbilityEnded.AddUObject(this, &UAC_EnemyMovementManager::OnMovementAbilityEnded);
	}
	else
	{
		MovementChainTracker.Advance();
		TryExecuteNextMovementAbilityInChain();
	}
}

void UAC_EnemyMovementManager::ApplyDirectionPoliciesToMovementAbility(UMovementSingleData* MovementAbilityData, FGameplayTag AttackDirection)
{
	if (!MovementAbilityData)
	{
		return;
	}

	FGameplayTag ResolvedDirectionTag;

	if (!MovementAbilityData->EnableDirectionPolicy || !MovementAbilityData->DirectionPolicyTag.IsValid())
	{
		MovementAbilityData->ResolvedDirectionTag = GetRandomDirectionTag();
		return;
	}

	if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_EscapeFromAttack)
	{
		ResolvedDirectionTag = ResolveAttackDirection(AttackDirection);
	}
	else if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
	{
		if (HeroMovementListener)
		{
			ResolvedDirectionTag = HeroMovementListener->GetHeroLastMovementDirectionTagByLastInput();
		}
	}
	else if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
	{
		ResolvedDirectionTag = GetRandomDirectionTag();
	}

	// FINAL GUARANTEE
	if (!ResolvedDirectionTag.IsValid())
	{
		ResolvedDirectionTag = GetRandomDirectionTag();
	}

	MovementAbilityData->ResolvedDirectionTag = ResolvedDirectionTag;
}

FGameplayTag UAC_EnemyMovementManager::GetRandomDirectionTag()
{
	static const TArray<FGameplayTag> PossibleDirections =
	{
		//GAS_Tags::TAG_AI_Direction_Resolved_Forward,
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



