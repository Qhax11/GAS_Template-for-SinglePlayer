// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TargetBase.h"
#include "GA_HeroHologramFinisher.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroHologramFinisher : public UGA_TargetBase
{
	GENERATED_BODY()

protected:
	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	virtual void OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result) override;

};
