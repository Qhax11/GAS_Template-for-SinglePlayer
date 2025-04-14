// Qhax's GAS Template for SinglePlayer

#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_Base.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UEQT_Base : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEQT_Base();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const;

	virtual void SetScoreToItem(FEnvQueryInstance::ItemIterator& Item, float Score) const;

	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	TSubclassOf<UEnvQueryContext> QuerierActorContext;

	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	float ScoringMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	float MaxExpectedScore = 3.0f;

protected:
	UPROPERTY()
	mutable AActor* QuerierActor = nullptr;

};
