// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_EyeOfView.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Tags/GAS_Tags.h"

USC_EyeOfView::USC_EyeOfView()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_EyeOfView::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
}

void USC_EyeOfView::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HeroBase || !PC)
	{
		return;
	}

	if(!HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting))
	{
		FVector Location;
		FRotator Rotation;
		PC->GetPlayerViewPoint(Location, Rotation);
		SetWorldRotation(Rotation);
	}

	FVector2D MouseInput;
	PC->GetInputMouseDelta(MouseInput.X, MouseInput.Y);

	if (!MouseInput.IsNearlyZero())
	{
		UpdateRotationFromMouseInput(MouseInput);
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

