// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/Shadows/ShadowTargetActorBase.h"
#include "BossShadowTargetActor.generated.h"


UCLASS()
class GAS_TEMPLATESP_API ABossShadowTargetActor : public AShadowTargetActorBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)  override;
};
