// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AC_EnemyMovementManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementChainEnded);


USTRUCT(BlueprintType)
struct FMovementChainData
{
    GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TriggerTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DirectionTag;
};

UCLASS(BlueprintType)
class UMovementChainSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMovementChainData> Movements;
};

USTRUCT(BlueprintType)
struct FAbilityMovementChain
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UMovementChainSet*> MovementChains;
};

UCLASS(BlueprintType)
class UAbilityMovementChainSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAbilityMovementChain> ChainMappings;
};

USTRUCT()
struct FMovementChainTracker
{
	GENERATED_BODY()

public:
	TArray<FMovementChainData> ActiveChain;
	int32 CurrentIndex = 0;
	bool bIsActive = false;

	void Start(const TArray<FMovementChainData>& InChain)
	{
		ActiveChain = InChain;
		CurrentIndex = 0;
		bIsActive = true;
	}

	void Reset()
	{
		ActiveChain.Empty();
		CurrentIndex = 0;
		bIsActive = false;
	}

	bool IsFinished() const
	{
		return !bIsActive || !ActiveChain.IsValidIndex(CurrentIndex);
	}

	const FMovementChainData* GetCurrent() const
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
	void StartMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass);

	UFUNCTION(BlueprintCallable)
	void CancelMovementAbilities();

protected:
	const TArray<FMovementChainData>* GetMovementChainForAbility(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass) const;

	void TryExecuteNextMovementAbilityInChain();

	void TryActivateMovementAbilityWithEventData(FMovementChainData MovementChainData);

	UFUNCTION()
	void OnMovementAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementManager")
	UAbilityMovementChainSet* AbilityMovementChainSet;

	UPROPERTY(BlueprintAssignable)
	FOnMovementChainEnded OnMovementChainEnded;
		
private:
	FMovementChainTracker MovementChainTracker;

	class AAIControllerBase* OwnerController;
	class AGAS_EnemyBase* OwnerEnemyBase;
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;
};
