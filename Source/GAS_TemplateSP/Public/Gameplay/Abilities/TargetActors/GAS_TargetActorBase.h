// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Abilities/GameplayAbilityTargetActor.h"
#include "GAS_TargetActorBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGAS_TargetActorBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:

	virtual void StartTargeting(UGameplayAbility* Ability) override;


};
