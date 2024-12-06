// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroControl.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroControl : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAC_HeroControl();

protected:

	virtual void BeginPlay() override;

public:	
	void TryBindControlInputs(UEnhancedInputComponent* EnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void LookMouse(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere)
	const UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere)
	const UInputAction* LookMouseInputAction;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastMovementInput;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastLookMouseInput;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AGAS_HeroBase* HeroBase;

	UAbilitySystemComponent* HeroASC;

private:
	
	float LastMovementInputTime = 0.0f; // The time when the last movement input was received
	const float MovementInputResetThreshold = 0.1f; // Threshold duration for resetting movement input (0.1 seconds)

	float LastLookMouseInputTime = 0.0f; // The time when the last movement input was received
	const float LookMouseInputResetThreshold = 0.1f; // Threshold duration for resetting movement input (0.1 seconds)
};
