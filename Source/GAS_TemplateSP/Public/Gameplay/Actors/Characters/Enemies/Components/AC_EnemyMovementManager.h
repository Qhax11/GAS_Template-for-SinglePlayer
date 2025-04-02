// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AC_EnemyMovementManager.generated.h"

USTRUCT(BlueprintType)
struct FMovementChainData
{
    GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TriggerTag;
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

protected:
	const TArray<FMovementChainData>* GetMovementChainForAbility(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass) const;

	void TryExecuteNextMovementAbilityInChain();

	void TryActivateMovementAbilityWithEventData(FMovementChainData MovementChainData);

	UFUNCTION()
	void OnMovementAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementManager")
	UAbilityMovementChainSet* AbilityMovementChainSet;
		
private:
	TArray<FMovementChainData> ActiveMovementChain;
	int32 CurrentMovementChainIndex = 0;

	class AAIControllerBase* OwnerController;
	class AGAS_EnemyBase* OwnerEnemyBase;
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;
};
