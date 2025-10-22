// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroJumpHandler.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"

UAC_HeroJumpHandler::UAC_HeroJumpHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_HeroJumpHandler::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s)"), *GetName());
		return;
	}

	HeroASC = HeroBase->GetAbilitySystemComponent();
	if (!HeroASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroASC is null in: %s)"), *GetName());
		return;
	}

	HeroControl = HeroBase->GetHeroControlComponent();
	if (!HeroControl)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroControl is null in: %s)"), *GetName());
		return;
	}

	HeroMovement = HeroBase->GetCharacterMovement();
	HeroAnimInstance = HeroBase->GetMesh() ? HeroBase->GetMesh()->GetAnimInstance() : nullptr;
	HeroBase->LandedDelegate.AddDynamic(this, &UAC_HeroJumpHandler::OnLanded);

	BindJumpInput();
}

bool UAC_HeroJumpHandler::BindJumpInput()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return false;
	}

	if (IA_ActivateJump)
	{
		EnhancedInputComponent->BindAction(IA_ActivateJump, ETriggerEvent::Triggered, this, &UAC_HeroJumpHandler::ActivateJump);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
		return false;
	}
}

void UAC_HeroJumpHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDebugPhases) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Current phase is: %s"), *UEnum::GetValueAsString(CurrentPhase));
	}
}

void UAC_HeroJumpHandler::ActivateJump()
{
	if (!HeroBase || !HeroMovement || !HeroControl)
	{
		return;
	}

	bool bCanJump = HeroMovement->IsMovingOnGround() || JumpCount < MaxJumpCount;
	if (!bCanJump) 
	{
		return;
	}

	if (PhaseTransitionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTransitionTimerHandle);
	}

	JumpCount++;

	if (JumpCount == 1) 
	{
		SetPhaseAndPlayMontage(EJumpPhase::Start);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				JumpLogic();
			}, 0.1, false);
	}
	else if (JumpCount == 2) 
	{
		JumpLogic();
		SetPhaseAndPlayMontage(EJumpPhase::DoubleJump);
	}
}

void UAC_HeroJumpHandler::JumpLogic()
{
	if (!HeroBase || !HeroMovement || !HeroControl)
	{
		return;
	}

	UGameplayEffect* GE_Ghost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GhostEffect);
	HeroASC->ApplyGameplayEffectToSelf(GE_Ghost, 1, FGameplayEffectContextHandle());

	// Kamera yönünü al (Controller'ýn rotation'ý)
	APlayerController* PC = Cast<APlayerController>(HeroBase->GetController());
	if (!PC)
	{
		return;
	}

	FRotator ControlRotation = PC->GetControlRotation();

	// Sadece yaw'ý kullan (pitch ve roll'u sýfýrla, yoksa yukarý/aþaðý bakarken garip olur)
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	// Kamera bazlý ileri ve sað vektörleri
	FVector CameraForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector CameraRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Input'u kamera yönüne göre hesapla
	FVector2D Input = HeroControl->LastMovementInput;
	FVector InputDirection = (CameraForward * Input.Y + CameraRight * Input.X).GetSafeNormal();

	float JumpVelocityZ = FMath::Sqrt(2.0f * FMath::Abs(HeroMovement->GetGravityZ()) * JumpHeight);

	if (JumpCount == 1) // Ýlk zýplama - yerden
	{
		if (!InputDirection.IsNearlyZero())
		{
			const float GroundJumpForwardStrength = 400.0f;
			FVector LaunchVelocity = InputDirection * GroundJumpForwardStrength + FVector(0, 0, JumpVelocityZ);
			HeroBase->LaunchCharacter(LaunchVelocity, false, true);
		}
		else
		{
			HeroBase->LaunchCharacter(FVector(0, 0, JumpVelocityZ), false, true);
		}
	}
	else if (JumpCount == 2) // Double jump - havada tam kontrol
	{
		FVector NewVelocity;

		if (!InputDirection.IsNearlyZero())
		{
			// Havada tamamen yeni yöne git - akrobatik kontrol
			const float AirControlStrength = 600.0f; // Güçlü kontrol
			NewVelocity = InputDirection * AirControlStrength + FVector(0, 0, JumpVelocityZ);
		}
		else
		{
			// Input yoksa mevcut yatay hýzý koru
			FVector CurrentVelocity = HeroMovement->Velocity;
			NewVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, JumpVelocityZ);
		}

		HeroBase->LaunchCharacter(NewVelocity, false, true);
	}
}

bool UAC_HeroJumpHandler::IsInAir() const
{
	return HeroMovement && HeroMovement->IsFalling();
}

void UAC_HeroJumpHandler::SetPhaseAndPlayWithDelay(EJumpPhase NewPhase, float Delay)
{
	if (PhaseTransitionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTransitionTimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(PhaseTransitionTimerHandle, [this, NewPhase]()
		{
			SetPhaseAndPlayMontage(NewPhase);
		}, Delay, false);
}

void UAC_HeroJumpHandler::SetPhaseAndPlayMontage(EJumpPhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}

	CurrentPhase = NewPhase;

	if (NewPhase == EJumpPhase::Start)
	{
		float JumpStartMontageLenght = PlayMontage(JumpStartMontage);
		SetPhaseAndPlayWithDelay(EJumpPhase::InAir, JumpStartMontageLenght);
	}
	else if (NewPhase == EJumpPhase::InAir)
	{
		PlayMontage(JumpInAirMontage);
	}
	else if (NewPhase == EJumpPhase::DoubleJump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Double Jump Phase Activated"));
		float DoubleJumpStartMontageLenght = PlayMontage(DoubleJumpStartMontage);
		SetPhaseAndPlayWithDelay(EJumpPhase::InAir, DoubleJumpStartMontageLenght - 0.3f);
	}
	else if (NewPhase == EJumpPhase::Landed)
	{
		PlayMontage(JumpLandedMontage);
	}
}

float UAC_HeroJumpHandler::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (!HeroAnimInstance || !MontageToPlay) 
	{
		return 0.0f;
	}

	return HeroAnimInstance->Montage_Play(MontageToPlay);
}

void UAC_HeroJumpHandler::OnLanded(const FHitResult& Hit)
{
	JumpCount = 0;
	SetPhaseAndPlayMontage(EJumpPhase::Landed);
	SetPhaseAndPlayWithDelay(EJumpPhase::None, 0.3f);
}

void UAC_HeroJumpHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}