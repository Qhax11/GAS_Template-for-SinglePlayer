// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
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

	const FMovementAbilityData* GetCurrentMovementAbilityInChain() const
	{
		return ActiveChain.IsValidIndex(CurrentIndex) ? &ActiveChain[CurrentIndex] : nullptr;
	}

	void Advance()
	{
		++CurrentIndex;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_EnemyMovementManager : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UAC_EnemyMovementManager();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

	UFUNCTION(BlueprintCallable)
	void StopMovementAbilities();

	void CancelMovementAbilities();

	UPROPERTY(BlueprintReadWrite)
	bool bLockedMovementChain = false;

protected:
	void TryExecuteNextMovementAbilityInChain();

	void TryActivateMovementAbilityWithEventData(FMovementAbilityData MovementChainData);

	UFUNCTION()
	void OnMovementAbilityEnded(const FAbilityEndedDataBP& AbilityEndedData);

	UPROPERTY(BlueprintAssignable)
	FOnMovementChainEnded OnMovementChainEnded;

private:
	FMovementChainTracker MovementChainTracker;

	class AAIControllerBase* OwnerController;
	class UAC_BehaviorDecision* BehaviorDecisionComp;
	class AGAS_EnemyBase* OwnerEnemyBase;
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;
};
