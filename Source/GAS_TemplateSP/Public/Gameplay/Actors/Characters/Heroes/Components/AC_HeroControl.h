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

	virtual void BeginPlay() override;

	void TryBindControlInputs();

	void Move(const FInputActionValue& Value);

	void LookMouse(const FInputActionValue& Value);

	void ClampingPitchValue(const float NewPitchValue, const float LookMouseValueY);

	UPROPERTY(EditAnywhere)
	const UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	const UInputAction* IA_LookMouse;

	UPROPERTY(EditAnywhere)
	const UInputAction* IA_ConfirmTarget;

	UPROPERTY(EditAnywhere)
	const UInputAction* IA_CancelTarget;

	UPROPERTY(EditDefaultsOnly, Meta = (ToolTip = " Minimum limit for looking down from below"))
	float MinPitchA = 0.0f;  

	UPROPERTY(EditDefaultsOnly, Meta = (ToolTip = "Maximum limit for looking down from below"))
	float MaxPitchA = 20.0f;  

	UPROPERTY(EditDefaultsOnly, Meta = (ToolTip = "Minimum limit for looking up from above"))
	float MinPitchB = 340.0f; 

	UPROPERTY(EditDefaultsOnly, Meta = (ToolTip = "Maximum limit for looking up from above"))
	float MaxPitchB = 360.0f; 

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastMovementInput;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastLookMouseInput;

	UPROPERTY(EditDefaultsOnly)
	float SnapRotationRate = 12000.f;

	UPROPERTY(EditDefaultsOnly)
	float DefaultRotationRate = 450.f;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AGAS_HeroBase* HeroBase;
	UAbilitySystemComponent* HeroASC;

private:
	UEnhancedInputComponent* EnhancedInputComponent;
	
	float LastMovementInputTime = 0.0f; // The time when the last movement input was received
	const float MovementInputResetThreshold = 0.1f; // Threshold duration for resetting movement input (0.1 seconds)

	float LastLookMouseInputTime = 0.0f; // The time when the last movement input was received
	const float LookMouseInputResetThreshold = 0.1f; // Threshold duration for resetting movement input (0.1 seconds)
};
