// Qhax's GAS Template for SinglePlayer

#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_BossStrafing.generated.h"


UENUM()
enum class EStrafeDirection : uint8
{
	Left,
	Right,
	Both
};

UCLASS()
class GAS_TEMPLATESP_API UEQT_BossStrafing : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEQT_BossStrafing();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const;

	EStrafeDirection GetDirectionFromParam(const FEnvQueryInstance& QueryInstance) const;
};
