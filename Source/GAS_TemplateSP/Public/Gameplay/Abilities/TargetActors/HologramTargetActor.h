// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "HologramTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API AHologramTargetActor : public AGAS_TargetActorBase
{
	GENERATED_BODY()

public:

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

};
