// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Cues/GCN_ActorBase.h"
#include "GCN_SoundBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGCN_SoundBase : public AGCN_ActorBase
{
	GENERATED_BODY()

public:
	virtual void OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) override;

	void PlaySoundForActor(AActor* Actor);
};
