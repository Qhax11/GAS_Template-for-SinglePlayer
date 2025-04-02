// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Animation/AnimInstanceBase.h"
#include "AnimInstanceEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UAnimInstanceEnemy : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsStrafing;
	
};
