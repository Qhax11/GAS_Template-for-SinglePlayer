// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Cues/GCN_ActorBase.h"
#include "GCN_CameraShakeBase.generated.h"

/**
 * Gameplay cue manager for handling combat-related sounds and camera shakes.
 * Triggers effects based on the source’s attack type.
 */

UCLASS()
class GAS_TEMPLATESP_API AGCN_CameraShakeBase : public AGCN_ActorBase
{
	GENERATED_BODY()

public:
	AGCN_CameraShakeBase();

    virtual void OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CameraShakeBase")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

};
