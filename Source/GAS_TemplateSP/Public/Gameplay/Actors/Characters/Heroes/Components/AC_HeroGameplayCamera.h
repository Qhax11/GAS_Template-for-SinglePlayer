// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroGameplayCamera.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_HeroGameplayCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroGameplayCamera();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CameraShake();
		
};
