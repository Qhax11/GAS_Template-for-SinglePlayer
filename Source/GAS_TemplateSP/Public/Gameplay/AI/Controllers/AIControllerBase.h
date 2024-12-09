// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class GAS_TEMPLATESP_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:

	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

protected:

	UFUNCTION()
	void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
