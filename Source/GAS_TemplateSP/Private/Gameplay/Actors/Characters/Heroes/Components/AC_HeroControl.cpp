// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	TryBindControlInputs();
}

void UAC_HeroControl::TryBindControlInputs()
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (IA_Move && IA_LookMouse)
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UAC_HeroControl::Move);
		EnhancedInputComponent->BindAction(IA_LookMouse, ETriggerEvent::Triggered, this, &UAC_HeroControl::LookMouse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
	}
}

void UAC_HeroControl::Move(const FInputActionValue& Value)
{
	if (!HeroBase || !HeroBase->Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase or Hero Controller are null in: %s"), *GetName());
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero())
	{
		return;
	}

	// New input received, update the LastMovementInput
	LastMovementInput = MovementVector;
	LastMovementInputTime = GetWorld()->GetTimeSeconds();

	// Find out which way is forward
	const FRotator Rotation = HeroBase->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	HeroBase->AddMovementInput(ForwardDirection, MovementVector.Y);
	HeroBase->AddMovementInput(RightDirection, MovementVector.X);
}

void UAC_HeroControl::LookMouse(const FInputActionValue& Value)
{
	const FVector2D LookMouseVector = Value.Get<FVector2D>();

	if (LookMouseVector.IsNearlyZero())
	{
		return;
	}

	// New input received, update the LastLookMouseInput
	LastLookMouseInput = LookMouseVector;
	LastLookMouseInputTime = GetWorld()->GetTimeSeconds();

	if (HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked)) 
	{
		return;
	}

	HeroBase->AddControllerYawInput(LookMouseVector.X);

	FRotator HeroControlRotation = HeroBase->GetControlRotation();
	float NewPitch = HeroControlRotation.Pitch + LookMouseVector.Y;
	ClampingPitchValue(NewPitch, LookMouseVector.Y);
}

void UAC_HeroControl::ClampingPitchValue(const float NewPitchValue, const float LookMouseValueY)
{
	// Normalize pitch to the range [0, 360]
	float NewPitch = FMath::Fmod(NewPitchValue + 360.0f, 360.0f);

	// Check for looking up from below, NewPitch value is between MinPitchA and MaxPitchA
	if (NewPitch >= MinPitchA && NewPitch <= MaxPitchA)
	{
		HeroBase->AddControllerPitchInput(LookMouseValueY);
	}
	// Check for looking down from above, NewPitch value is between MinPitchB and MaxPitchB
	else if (NewPitch >= MinPitchB && NewPitch <= MaxPitchB)
	{
		HeroBase->AddControllerPitchInput(LookMouseValueY);
	}
	// NewPitch value is between MaxPitchA and MinPitchB
	else 
	{
		float NewPitchDistanceToMaxPitchA = FMath::Abs(NewPitch - MaxPitchA);
		float NewPitchDistanceToMinPitchB = FMath::Abs(NewPitch - MinPitchB);
		// If closer to MaxPitchA degrees 
		if (NewPitchDistanceToMaxPitchA < NewPitchDistanceToMinPitchB && LookMouseValueY > 0)
		{
			HeroBase->AddControllerPitchInput(LookMouseValueY);
		}
		// If closer to MinPitchB degrees
		else if (NewPitchDistanceToMaxPitchA > NewPitchDistanceToMinPitchB && LookMouseValueY < 0)
		{
			HeroBase->AddControllerPitchInput(LookMouseValueY);
		}
	}
}

void UAC_HeroControl::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If more than the threshold time has passed since the last movement input
	if (GetWorld()->GetTimeSeconds() - LastMovementInputTime > MovementInputResetThreshold)
	{
		LastMovementInput = FVector2D::ZeroVector; 
	}

	// If more than the threshold time has passed since the last movement input
	if (GetWorld()->GetTimeSeconds() - LastLookMouseInputTime > LookMouseInputResetThreshold)
	{
		LastLookMouseInput = FVector2D::ZeroVector;
	}

	CharacterTurn(DeltaTime);
}

void UAC_HeroControl::CharacterTurn(float DeltaTime)
{
	bool HeroTargetLocked = HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
	bool HeroFinisher = HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);

	// Target lock veya finisher aktifse cache'i temizle ve çýk
	if (HeroTargetLocked || HeroFinisher)
	{
		bHasDesiredRotation = false; // Cache'i temizle!
		return;
	}

	if (!HeroBase || !HeroBase->GetCharacterMovement())
	{
		return;
	}

	FRotator CurrentRotation = HeroBase->GetActorRotation();
	FVector MovementInput = HeroBase->GetCharacterMovement()->GetLastInputVector();
	bool bHasInput = !MovementInput.IsNearlyZero();

	if (bHasInput)
	{
		FRotator DesiredRotation = MovementInput.GetSafeNormal().Rotation();
		CachedDesiredRotation = DesiredRotation;
		bHasDesiredRotation = true;
	}

	if (!bHasDesiredRotation)
	{
		return;
	}

	float RotationRate = HeroBase->GetCharacterMovement()->RotationRate.Yaw;
	float NewYaw = FMath::FixedTurn(CurrentRotation.Yaw, CachedDesiredRotation.Yaw, RotationRate * DeltaTime);

	if (FMath::IsNearlyEqual(NewYaw, CachedDesiredRotation.Yaw, 1.f))
	{
		bHasDesiredRotation = false;
	}

	HeroBase->SetActorRotation(FRotator(0, NewYaw, 0));
}