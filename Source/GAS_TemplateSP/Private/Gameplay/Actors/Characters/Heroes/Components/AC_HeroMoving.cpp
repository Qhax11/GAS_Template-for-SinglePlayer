// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMoving.h"
#include "GameFramework/PlayerController.h"


UAC_HeroMoving::UAC_HeroMoving()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroMoving::BeginPlay()
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

void UAC_HeroMoving::TryBindMovingInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *this->GetName());
		return;
	}

	if (LeftInputAction && RightInputAction && ForwardInputAction && BackwardInputAction)
	{
		EnhancedInputComponent->BindAction(LeftInputAction, ETriggerEvent::Triggered, this, &UAC_HeroMoving::MoveLeft);
		EnhancedInputComponent->BindAction(RightInputAction, ETriggerEvent::Triggered, this, &UAC_HeroMoving::MoveRight);
		EnhancedInputComponent->BindAction(ForwardInputAction, ETriggerEvent::Triggered, this, &UAC_HeroMoving::MoveForward);
		EnhancedInputComponent->BindAction(BackwardInputAction, ETriggerEvent::Triggered, this, &UAC_HeroMoving::MoveBackward);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *this->GetName());
	}
}

void UAC_HeroMoving::MoveLeft(const FInputActionValue& Value)
{
	HeroBase->AddMovementInput(FVector(0, -1, 0));
}

void UAC_HeroMoving::MoveRight(const FInputActionValue& Value)
{
	HeroBase->AddMovementInput(FVector(0, 1, 0));
}

void UAC_HeroMoving::MoveForward(const FInputActionValue& Value)
{
	HeroBase->AddMovementInput(FVector(1, 0, 0));
}

void UAC_HeroMoving::MoveBackward(const FInputActionValue& Value)
{
	HeroBase->AddMovementInput(FVector(-1, 0, 0));
}

