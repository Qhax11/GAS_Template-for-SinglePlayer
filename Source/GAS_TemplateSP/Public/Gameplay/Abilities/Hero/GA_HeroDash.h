// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_HeroDash.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDash : public UGA_MontageAbility
{
	GENERATED_BODY()
	

protected:

	virtual void OnEventReceived() override;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionTaskParams")
	float Strenght = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionTaskParams")
	float Duration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionTaskParams")
	UCurveFloat* DashCurve;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionTaskParams")
	bool bEnableGravity;

};
