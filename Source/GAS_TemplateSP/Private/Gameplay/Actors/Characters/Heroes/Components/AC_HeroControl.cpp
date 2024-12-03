// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "GameFramework/PlayerController.h"


UAC_HeroControl::UAC_HeroControl()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroControl::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null! (This message comes from %s)"), *GetName());
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	TryBindMovingInputs(EnhancedInputComponent);
}

void UAC_HeroControl::TryBindMovingInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (MoveInputAction && LookMouseInputAction)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &UAC_HeroControl::Move);
		EnhancedInputComponent->BindAction(LookMouseInputAction, ETriggerEvent::Triggered, this, &UAC_HeroControl::LookMouse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
	}
}

void UAC_HeroControl::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (HeroBase != nullptr && HeroBase->Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = HeroBase->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		HeroBase->AddMovementInput(ForwardDirection, MovementVector.Y);
		HeroBase->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UAC_HeroControl::LookMouse(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D VectorValue = Value.Get<FVector2D>();

	if (VectorValue.X != 0.0f)
	{
		HeroBase->AddControllerYawInput(VectorValue.X);
	}

	if (VectorValue.Y != 0.0f)
	{
		HeroBase->AddControllerPitchInput(VectorValue.Y);
	}
}

