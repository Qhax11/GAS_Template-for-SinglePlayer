// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_TargetLockSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GAS_TEMPLATESP_API UAC_TargetLockSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_TargetLockSystem();

	UPROPERTY(EditAnywhere)
	const UInputAction* LookMouseInputAction;

	UPROPERTY(EditAnywhere)
	float Threshold;

protected:
	virtual void BeginPlay() override;

	void TryBindLookMouseInputs(UEnhancedInputComponent* EnhancedInputComponent);

	void LookMouse(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void TargetChange(bool right);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	AGAS_HeroBase* HeroBase;
		
};
