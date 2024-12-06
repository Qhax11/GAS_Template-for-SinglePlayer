// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_HeroControl::UAC_HeroControl()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_HeroControl::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s, cannot initialize HeroControl"), *GetName());
		return;
	}

	HeroASC = HeroBase->GetAbilitySystemComponent();
	if (!HeroASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroASC is null in %s, cannot initialize HeroControl."), *this->GetName());
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	TryBindControlInputs(EnhancedInputComponent);
}

void UAC_HeroControl::TryBindControlInputs(UEnhancedInputComponent* EnhancedInputComponent)
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
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (HeroBase != nullptr && HeroBase->Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = HeroBase->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		HeroBase->AddMovementInput(ForwardDirection, MovementVector.Y);
		HeroBase->AddMovementInput(RightDirection, MovementVector.X);
	}

	if (!MovementVector.IsNearlyZero())
	{
		// New input received, update the last movement input direction
		LastMovementInputDirection = MovementVector;
		LastMovementInputTime = GetWorld()->GetTimeSeconds(); 
	}
}

void UAC_HeroControl::LookMouse(const FInputActionValue& Value)
{
	if (HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Hero_TargetLocked)) 
	{
		return;
	}

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

void UAC_HeroControl::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If more than the threshold time has passed since the last movement input
	if (GetWorld()->GetTimeSeconds() - LastMovementInputTime > MovementInputResetThreshold)
	{
		LastMovementInputDirection = FVector2D::ZeroVector; // Input'u sýfýrla
	}
}

