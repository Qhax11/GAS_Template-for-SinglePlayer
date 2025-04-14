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

	float GetBoundFloatValue(const FEnvQueryInstance& QueryInstance, const FAIDataProviderFloatValue& Value) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	TSubclassOf<UEnvQueryContext> TargetActorContext;

	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	FAIDataProviderFloatValue ScoringMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "EQTBase")
	FAIDataProviderFloatValue MaxExpectedScore;
};
