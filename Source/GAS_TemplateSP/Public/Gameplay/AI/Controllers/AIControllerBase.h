// Qhax's GAS Template for SinglePlayer

#pragma once

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UST_Base> StateTreeAIComponent;

protected:
	virtual void BeginPlay();

	UFUNCTION()
	void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detaour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StateTree|Params")
	AActor* TargetActor;

};
