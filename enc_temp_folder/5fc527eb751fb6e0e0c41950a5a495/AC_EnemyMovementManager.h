// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_EnemyMovementManager.generated.h"

class UMovementDataBase;
class UMovementSingleData;
class UMovementChainDataa;

enum class EMovementExecutionType : uint8
{
	Reaction,
	Corrective,
	Chain
};

struct FMovementExecutionEndedData
{
	bool bWasCancelled = false;
	EMovementExecutionType ExecutionType = EMovementExecutionType::Chain;
	UMovementSingleData* MovementData = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementChainEnded);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementExecutionEnded, const FMovementExecutionEndedData&);

USTRUCT()
struct FMovementChainTracker
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UGAS_GameplayAbilityBase* CurrentMovementAbility = nullptr;
	TArray<UMovementSingleData*> ActiveChain;
	int32 CurrentIndex = 0;
	bool bIsActive = false;

	void StartChain(const TArray<UMovementSingleData*>& InChain)
	{
		ActiveChain = InChain;
		CurrentIndex = 0;
		bIsActive = true;
	}

	void ResetChain()
	{
		ActiveChain.Empty();
		CurrentIndex = 0;
		bIsActive = false;
	}

	bool IsChainFinished() const
	{
		return !bIsActive || !ActiveChain.IsValidIndex(CurrentIndex);
	}

	UMovementSingleData* GetCurrentMovementAbilityInChain() const
	{
		return ActiveChain.IsValidIndex(CurrentIndex) ? ActiveChain[CurrentIndex] : nullptr;
	}

	void Advance()
	{
		++CurrentIndex;
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

	// Reaction context (dodge, evade, panic, vs)
	bool ExecuteReactionMovement(UMovementSingleData* MovementData, const FComingAttackPayload& AttackPayload);

	bool ExecuteCorrectiveMovement(UMovementSingleData* MovementData);

	FOnMovementChainEnded OnMovementChainEnded;

	FOnMovementExecutionEnded OnMovementExecutionEnded;

private:
	bool ValidateMovementData(UMovementSingleData* MovementData) const;

private:
	// Generic activation - callback type belirler hangi flow'da olduðumuzu
	UGAS_GameplayAbilityBase* ActivateAndBindMovementAbility(
		UMovementSingleData* MovementData,
		EMovementExecutionType ExecutionType,
		const FComingAttackPayload& AttackPayload = FComingAttackPayload()
	);

	void OnMovementAbilityExecutionEnded(const FCustomAbilityEndedData& EndData);

	// For now, it's dodge.
	UGAS_GameplayAbilityBase* ActivatedReactionAbility;

public:
	UFUNCTION(BlueprintCallable)
	void StopMovementAbilities();

	void CancelMovementAbilities();

	void ApplyDirectionPoliciesToMovementAbility(UMovementSingleData* MovementAbilityData, const FComingAttackPayload& AttackPayload = FComingAttackPayload());

protected:
	void TryExecuteNextMovementAbilityInChain();

	void TryActivateMovementAbilityWithEventData(UMovementSingleData* MovemenData);

	FGameplayTag GetRandomDirectionTag();

	// Resolves the final movement direction based on the incoming attack direction.
    // Applies attack-direction–specific rules (e.g. horizontal vs vertical attacks)
    // and returns a single, safe direction tag to be used by movement abilities.
    // May return an invalid tag if the input is invalid and no fallback is applied.
	FGameplayTag ResolveAttackDirection(FGameplayTag AttackDirectionTag);

	UFUNCTION()
	void OnMovementAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData);

private:
	FMovementChainTracker MovementChainTracker;

	UPROPERTY()
	UAC_HeroMovementListener* HeroMovementListener;
};
