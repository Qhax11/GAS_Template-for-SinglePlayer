// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DashBase.h"
#include "GA_BossDash.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_BossDash : public UGA_DashBase
{
	GENERATED_BODY()
	
protected:

	virtual FVector CalculateDestination() override;


};
