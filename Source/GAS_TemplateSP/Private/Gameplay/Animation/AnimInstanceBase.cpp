// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/AnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "KismetAnimationLibrary.h"

UAnimInstanceBase::UAnimInstanceBase()
{
	Speed = 0.f;
	bIsInAir = false;
	bIsAccelerating = false;
}

void UAnimInstanceBase::NativeInitializeAnimation()
{
	CharacterRef = Cast<ACharacter>(TryGetPawnOwner());
	if (CharacterRef)
	{
		CharacterMovement = CharacterRef->GetCharacterMovement();
	}
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CharacterRef || !CharacterMovement)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterRef or CharacterMovement is null in: %s"), *GetName());
		return;
	}

	FVector Velocity = CharacterRef->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	LocomationDirection = UKismetAnimationLibrary::CalculateDirection(CharacterRef->GetVelocity(), CharacterRef->GetActorRotation());

	bIsInAir = CharacterMovement->IsFalling();

	if (CharacterMovement->GetCurrentAcceleration().Size() > 0.f)
	{
		bIsAccelerating = true;
	}
	else
	{
		bIsAccelerating = false;
	}
}
