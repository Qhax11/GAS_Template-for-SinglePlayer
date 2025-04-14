// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/EQS/Tests/EQT_Base.h"
#include "EQT_DotDirection.generated.h"


UENUM()
enum class EStrafeDirection : uint8
{
	Left,
	Right,
	Both
};

UCLASS()
class GAS_TEMPLATESP_API UEQT_DotDirection : public UEQT_Base
{
	GENERATED_BODY()

public:
	UEQT_DotDirection();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	EStrafeDirection GetDirectionFromParam(const FEnvQueryInstance& QueryInstance) const;

	float ApplyFrontPenalty(float StrafingScore, float AngleDot) const;

	UPROPERTY(EditDefaultsOnly, Category = "DotDirection")
	float FrontPenaltyStrength = 50.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "DotDirection|Filter")
	float FrontCutoffDotThreshold = 1.0f;
};
