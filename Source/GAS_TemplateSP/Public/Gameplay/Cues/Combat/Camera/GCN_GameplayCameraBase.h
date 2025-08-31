// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Cues/Combat/GCN_CombatBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroGameplayCamera.h"
#include "GCN_GameplayCameraBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGCN_GameplayCameraBase : public AGCN_CombatBase
{
	GENERATED_BODY()
	
public:
	virtual bool OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) override;

protected:
	UAC_HeroGameplayCamera* HeroGameplayCameraComponent;

};
