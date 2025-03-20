// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_MovementListener.generated.h"

// Recommendation: make function that calculate how far have you traveled for ai.

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_MovementListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_MovementListener();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float LastMovementTime = 0.0f;

	UFUNCTION(BlueprintCallable)
	bool HasMovedInLastSeconds(float Seconds) const;
};
