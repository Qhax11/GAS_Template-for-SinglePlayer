// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "A_TutorialGateBase.generated.h"

UCLASS()
class GAS_TEMPLATESP_API AA_TutorialGateBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AA_TutorialGateBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenGate();
};
