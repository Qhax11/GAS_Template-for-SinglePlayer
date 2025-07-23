// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "AIControllerBoss.generated.h"



UCLASS()
class GAS_TEMPLATESP_API AAIControllerBoss : public AAIControllerBase
{
	GENERATED_BODY()
	
protected:
	AAIControllerBoss(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	virtual void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	virtual bool ShouldUpdateRotation() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAC_IntendHandlerBoss> IntendHandlerBossComponent;
};
