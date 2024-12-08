// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_EyeOfView.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

USC_EyeOfView::USC_EyeOfView()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_EyeOfView::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
}

void USC_EyeOfView::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector2D MouseInput;
	if (PC)
	{
		PC->GetInputMouseDelta(MouseInput.X, MouseInput.Y);

		if (!MouseInput.IsNearlyZero())
		{
			UpdateRotationFromMouseInput(MouseInput);
		}
	}
}

void USC_EyeOfView::UpdateRotationFromMouseInput(const FVector2D& MouseInput)
{
	FRotator DeltaRot;
	DeltaRot.Pitch = MouseInput.Y * RotationSpeed; 
	DeltaRot.Yaw = MouseInput.X * RotationSpeed;   
	DeltaRot.Roll = 0.0f;                          

	FRotator CurrentRotation = GetComponentRotation();

	FRotator NewRotation = CurrentRotation + DeltaRot;

	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -89.9f, 89.9f);

	SetWorldRotation(NewRotation);
}

