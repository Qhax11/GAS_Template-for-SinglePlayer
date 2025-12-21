// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_EnemyMovementManager.generated.h"

class UMovementDataBase;
class UMovementChainData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementChainEnded);

USTRUCT()
struct FMovementChainTracker
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UGAS_GameplayAbilityBase* CurrentMovementAbility = nullptr;
	TArray<UMovementSingleData*> ActiveChain;
	int32 CurrentIndex = 0;

	void StartChain(const TArray<UMovementSingleData*>& InChain)
	{
		ActiveChain = InChain;
		CurrentIndex = 0;
	}

	void ResetChain()
	{
		ActiveChain.Empty();
		CurrentIndex = 0;
		CurrentMovementAbility = nullptr;
	}

	bool IsChainFinished() const
	{
		return GetCurrentValidMovement() == nullptr;
	}

	UMovementSingleData* GetCurrentMovementAbilityInChain() const
	{
		return ActiveChain.IsValidIndex(CurrentIndex) ? ActiveChain[CurrentIndex] : nullptr;
	}

	void Advance()
	{
		++CurrentIndex;
	}

	UMovementSingleData* GetCurrentValidMovement() const
	{
		for (int32 Index = CurrentIndex; Index < ActiveChain.Num(); ++Index)
		{
			UMovementSingleData* Data = ActiveChain[Index];
			if (IsValid(Data) && Data->IsValidData())
			{
				return Data;
			}
		}
		return nullptr;
	}
};

class UAC_HeroMovementListener;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_EnemyMovementManager : public UAC_EnemyBase
{
	GENERATED_BODY()

protected:	
	UAC_EnemyMovementManager();

	virtual void BeginPlay() override;

	/*===============  PUBLIC API ===============*/
public:
	UFUNCTION(BlueprintCallable)
	void ExecuteMovementChain(UMovementChainData* MovementChain);

	// Executes a defensive reaction movement in response to an incoming attack (e.g. dodge, evade, panic).
	UGAS_GameplayAbilityBase* ExecuteReactionMovement(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload);

	// Executes a non-reactive corrective movement to adjust positioning (e.g. step-back, micro-reposition).
	UGAS_GameplayAbilityBase* ExecuteCorrectiveMovement(UMovementSingleData* MovementData);

private:
	void TryExecuteNextMovementAbilityInChain();

	// Generic activation 
	UGAS_GameplayAbilityBase* ActivateMovementAbility(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload = FComingAttackPayload());

	/*===============  HELPERS ===============*/
	void ApplyDirectionPoliciesToMovementAbility(UMovementSingleData* MovementAbilityData, const FComingAttackPayload& AttackPayload = FComingAttackPayload());

	FGameplayTag ResolveDirectionFromPolicy(const FGameplayTag& PolicyTag, const FComingAttackPayload& AttackPayload);

	FGameplayTag GetRandomStrafeDirectionTag();

	FGameplayTag GetRandomRepositionDirectionTag();

	// Resolves the final movement direction based on the incoming attack direction.
	FGameplayTag ResolveAttackDirection(FGameplayTag AttackDirectionTag);

	/*===============  PUBLIC API ===============*/
public:
	UFUNCTION(BlueprintCallable)
	void StopChain();

	void InterruptByReaction();

	FOnMovementChainEnded OnMovementChainEnded;

private:
	/*===============  CALLBACKS ===============*/
	UFUNCTION()
	void OnMovementAbilityInChainEnded(const FCustomAbilityEndedData& AbilityEndedData);

	/*===============  STATE ===============*/
	UPROPERTY()
	UAC_HeroMovementListener* HeroMovementListener;

	// For now, it's dodge.
	UGAS_GameplayAbilityBase* ActivatedReactionAbility;
	FMovementChainTracker MovementChainTracker;
};
