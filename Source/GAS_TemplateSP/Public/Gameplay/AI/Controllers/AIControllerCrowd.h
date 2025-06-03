// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "AIControllerCrowd.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API AAIControllerCrowd : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AAIControllerCrowd(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAC_IntendHandlerCrowd> IntendHandlerCrowdComponent;

};
