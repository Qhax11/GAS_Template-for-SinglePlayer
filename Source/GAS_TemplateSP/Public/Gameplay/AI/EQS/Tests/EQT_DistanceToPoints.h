// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/EQS/Tests/EQT_Base.h"
#include "EQT_DistanceToPoints.generated.h"

UENUM(BlueprintType)
enum class EDistanceScoringMode : uint8
{
	Linear,
	InverseLinear
};

UCLASS()
class GAS_TEMPLATESP_API UEQT_DistanceToPoints : public UEQT_Base
{
	GENERATED_BODY()
	
public:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DistanceToPoints")
	EDistanceScoringMode ScoringMode = EDistanceScoringMode::Linear;

	UPROPERTY(EditDefaultsOnly, Category = "DistanceToPoints")
	float MinDistance = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DistanceToPoints")
	float MaxDistance = 1000.0f;
};
