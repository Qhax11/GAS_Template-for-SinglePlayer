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

	void CharacterTurn(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = "HeroController|HeroRotation")
	bool bOrientRotationToMovement = true;

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|HeroRotation")
	float MinRotationRate = 360.f;

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|HeroRotation")
	float MaxRotationRate = 720.f;

	void LookMouse(const FInputActionValue& Value);

	void ClampingPitchValue(const float NewPitchValue, const float LookMouseValueY);

	UPROPERTY(EditAnywhere, Category = "HeroController|Input")
	const UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "HeroController|Input")
	const UInputAction* IA_LookMouse;

	UPROPERTY(EditAnywhere, Category = "HeroController|Input")
	const UInputAction* IA_ConfirmTarget;

	UPROPERTY(EditAnywhere, Category = "HeroController|Input")
	const UInputAction* IA_CancelTarget;

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|CameraRotation", Meta = (ToolTip = " Minimum limit for looking down from below"))
	float MinPitchA = 0.0f;  

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|CameraRotation", Meta = (ToolTip = "Maximum limit for looking down from below"))
	float MaxPitchA = 10.0f;  

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|CameraRotation", Meta = (ToolTip = "Minimum limit for looking up from above"))
	float MinPitchB = 340.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "HeroController|CameraRotation", Meta = (ToolTip = "Maximum limit for looking up from above"))
	float MaxPitchB = 360.0f; 

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastMovementInput;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastLookMouseInput;

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

	FRotator CachedDesiredRotation;
	bool bHasDesiredRotation = false;

	static constexpr float MaxAngle = 180.f;
};
