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
	if (!ValidateMovementChainData(MovementChain)) 
	{
		return;
	}

	MovementChainTracker.StartChain(MovementChain->MovementChain);
	TryExecuteNextMovementAbilityInChain();
}

UGAS_GameplayAbilityBase* UAC_EnemyMovementManager::ExecuteReactionMovement(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload)
{
	if (!ValidateMovementData(MovementData))
	{
		return nullptr;
	}

	// Reaction chain interrupt everything
	StopChain();

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
	if (!ValidateMovementData(MovementData))
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
		OnMovementChainEnded.Broadcast();
		StopChain();
		return;
	}

	UGAS_GameplayAbilityBase* MovementAbilityInChain = ActivateMovementAbility(MovementDataInChain);
	if (!MovementAbilityInChain)
	{
		UE_LOG(LogTemp, Log, 
			TEXT("Execution: Movement: UAC_EnemyMovementManager: MovementInChain activation failed, try next data!"));
		MovementChainTracker.Advance();
		TryExecuteNextMovementAbilityInChain();
	}
	else
	{
		MovementAbilityInChain->OnAbilityEnded.RemoveAll(this);
		MovementAbilityInChain->OnAbilityEnded.AddUObject(this, &UAC_EnemyMovementManager::OnMovementAbilityInChainEnded);
		MovementChainTracker.CurrentMovementAbility = MovementAbilityInChain;
	}
}

UGAS_GameplayAbilityBase* UAC_EnemyMovementManager::ActivateMovementAbility(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload)
{
	if (!ValidateMovementData(MovementData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: Validation failed!"));
		return nullptr;
	}

	// Step 1: Apply direction policies (attack direction used if relevant)
	ApplyDirectionPoliciesToMovementAbility(MovementData, AttackPayload);

	// Step 2: Build event data
	FGameplayEventData EventData;
	EventData.EventTag = MovementData->AbilityTriggerTag;
	EventData.InstigatorTags.AddTag(MovementData->DirectionTag);
	EventData.EventMagnitude = MovementData->ExpectedDuration;

	return OwnerEnemyASC->TryActivateAbilityByClassWithEventData(MovementData->MovementAbilityClass, EventData);
}

bool UAC_EnemyMovementManager::ValidateMovementData(UMovementSingleData* MovementData) const
{
	if (!MovementData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: MovementData is null!"));
		return false;
	}

	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: OwnerEnemyASC is null!"));
		return false;
	}

	if (!MovementData->AbilityTriggerTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: AbilityTriggerTag is invalid!"));
		return false;
	}

	if (!MovementData->MovementAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: MovementAbilityClass is null!"));
		return false;
	}

	return true;
}

bool UAC_EnemyMovementManager::ValidateMovementChainData(UMovementChainData* MovementChainData) const
{
	if (!MovementChainData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: MovementData is null!"));
		return false;
	}

	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Execution: Movement: UAC_EnemyMovementManager: OwnerEnemyASC is null!"));
		return false;
	}

	return true;
}

void UAC_EnemyMovementManager::ApplyDirectionPoliciesToMovementAbility(UMovementSingleData* MovementAbilityData, const FComingAttackPayload& AttackPayload)
{
	if (!MovementAbilityData)
	{
		return;
	}

	// POLICY KAPALI → sadece garanti ver
	if (!MovementAbilityData->EnableDirectionPolicy || !MovementAbilityData->DirectionPolicyTag.IsValid())
	{
		if (!MovementAbilityData->DirectionTag.IsValid())
		{
			MovementAbilityData->DirectionTag = GetRandomDirectionTag();
		}
		return;
	}

	FGameplayTag FinalTag;
	FGameplayTag AttackDirection = AttackPayload.AttackDirectionTag;

	if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_EscapeFromAttack)
	{
		FinalTag = ResolveAttackDirection(AttackDirection);
	}
	else if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
	{
		if (HeroMovementListener)
		{
			FinalTag = HeroMovementListener->GetHeroLastMovementDirectionTagByLastInput();
		}
	}
	else if (MovementAbilityData->DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
	{
		FinalTag = GetRandomDirectionTag();
	}

	// FINAL GUARANTEE
	if (!FinalTag.IsValid())
	{
		FinalTag = GetRandomDirectionTag();
	}

	MovementAbilityData->DirectionTag = FinalTag;
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

void UAC_EnemyMovementManager::OnMovementAbilityInChainEnded(const FCustomAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.AbilityThatEnded != MovementChainTracker.CurrentMovementAbility)
	{
		return;
	}

	MovementChainTracker.CurrentMovementAbility = nullptr;

	if (AbilityEndedData.bWasCancelled)
	{
		UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_EnemyMovementManager: Chain cancelled by %s. Resetting."), *AbilityEndedData.AbilityThatEnded->GetName());
		StopChain();
		return;
	}

	MovementChainTracker.Advance();

	if (MovementChainTracker.IsChainFinished())
	{
		StopChain();
		OnMovementChainEnded.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("Execution: Movement: UAC_EnemyMovementManager: Chain is finished."), *AbilityEndedData.AbilityThatEnded->GetName());
		return;
	}

	TryExecuteNextMovementAbilityInChain();
}

void UAC_EnemyMovementManager::StopChain()
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
