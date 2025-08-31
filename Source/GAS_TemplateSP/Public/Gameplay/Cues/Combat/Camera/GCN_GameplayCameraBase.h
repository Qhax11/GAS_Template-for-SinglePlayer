// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Cues/Combat/GCN_CombatBase.h"
#include "GCN_GameplayCameraBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGCN_GameplayCameraBase : public AGCN_CombatBase
{
	GENERATED_BODY()
	
public:
	virtual void OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) override;


};
