// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_EnemyMovementManager.generated.h"

USTRUCT(BlueprintType)
struct FMovementChainData
{
    GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGAS_GameplayAbilityBase> MovementAbilityClass;
};


USTRUCT(BlueprintType)
struct FAbilityMovementChain
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UGAS_GameplayAbilityBase> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMovementChainData> Movements;
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

		
};
