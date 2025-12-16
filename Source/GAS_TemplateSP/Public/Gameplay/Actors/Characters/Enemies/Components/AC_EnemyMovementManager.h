// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyBase.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AC_EnemyMovementManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementChainEnded);

USTRUCT()
struct FMovementChainTracker
{
	GENERATED_BODY()

public:
	TArray<FMovementAbilityData> ActiveChain;
	UGAS_GameplayAbilityBase* CurrentMovementAbility = nullptr;
	int32 CurrentIndex = 0;
	bool bIsActive = false;

	void StartChain(const TArray<FMovementAbilityData>& InChain)
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

	bool IsCurrentAbilityStillValid() const
	{
		return bIsActive && CurrentMovementAbility && !CurrentMovementAbility->IsActive();
	}

	const FMovementAbilityData* GetCurrentMovementAbilityInChain() const
	{
		return ActiveChain.IsValidIndex(CurrentIndex) ? &ActiveChain[CurrentIndex] : nullptr;
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

public:
	UFUNCTION(BlueprintCallable)
	void StartMovementChain(UMovementChainAsset* MovementChain);

	UFUNCTION(BlueprintCallable)
	void StopMovementAbilities();

	void CancelMovementAbilities();

	void ApplyDirectionPoliciesToMovementAbility(FMovementAbilityData& MovementAbility, FGameplayTag AttackDirection = FGameplayTag());

	UPROPERTY(BlueprintAssignable)
	FOnMovementChainEnded OnMovementChainEnded;

protected:
	void TryExecuteNextMovementAbilityInChain();

	void TryActivateMovementAbilityWithEventData(FMovementAbilityData MovementChainData);

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
