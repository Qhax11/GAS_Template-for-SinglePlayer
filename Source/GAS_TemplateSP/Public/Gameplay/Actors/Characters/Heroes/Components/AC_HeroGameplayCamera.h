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

	void ShakeCamera(float Force);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CameraShake();
		
private:
	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	class AGAS_HeroBase* OwnerHero;
	class APlayerController* OwnerHeroPS;
};
