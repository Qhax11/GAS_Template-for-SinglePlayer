// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "AIControllerBoss.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AAIControllerBoss : public AAIControllerBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();

	virtual void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	// Tag Listen Functions
	UFUNCTION()
	void OnPlayerStartedAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
};
