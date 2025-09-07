// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/SceneComponent.h"
#include "SC_FinisherMotionWarpLocation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API USC_FinisherMotionWarpLocation : public USceneComponent
{
	GENERATED_BODY()

public:	
	USC_FinisherMotionWarpLocation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.MeleeFinisher"))
	FGameplayTag FinisherTag;

};
