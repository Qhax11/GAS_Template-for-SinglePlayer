// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroJumpHandler.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_HeroJumpHandler::UAC_HeroJumpHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
	JumpMontageBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo);
	JumpMontageBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_AirKick);
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
	if (HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage))
	{
		return;
	}

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
			}, 0.05f, false);
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

	if (JumpCount == 1) 
	{
		FVector NewVelocity;

		if (InputDirection.IsNearlyZero())
		{
			NewVelocity = FVector(0, 0, JumpVelocityZ);
		}
		else
		{
			NewVelocity = InputDirection * GroundJumpForwardStrength + FVector(0, 0, JumpVelocityZ);
		}

		HeroBase->LaunchCharacter(NewVelocity, false, true);
	}
	else if (JumpCount == 2)
	{
		FVector NewVelocity;

		if (InputDirection.IsNearlyZero())
		{
			FVector CurrentVelocity = HeroMovement->Velocity;
			NewVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, JumpVelocityZ);
		}
		else
		{
			NewVelocity = InputDirection * AirControlStrength + FVector(0, 0, JumpVelocityZ);
		}

		HeroBase->LaunchCharacter(NewVelocity, true, true);
	}
}

bool UAC_HeroJumpHandler::IsInAir() const
{
	return HeroMovement && HeroMovement->IsFalling();
}

void UAC_HeroJumpHandler::SetPhaseAndPlayMontageWithDelay(EJumpPhase NewPhase, float Delay)
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
	if (!SetPhase(NewPhase)) 
	{
		return;
	}

	if (NewPhase == EJumpPhase::Start)
	{
		float JumpStartMontageLenght = PlayMontage(JumpStartMontage);
		SetPhaseAndPlayMontageWithDelay(EJumpPhase::InAir, JumpStartMontageLenght);
	}
	else if (NewPhase == EJumpPhase::InAir)
	{
		PlayMontage(JumpInAirMontage);
	}
	else if (NewPhase == EJumpPhase::DoubleJump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Double Jump Phase Activated"));
		float DoubleJumpStartMontageLenght = PlayMontage(DoubleJumpStartMontage);
		SetPhaseAndPlayMontageWithDelay(EJumpPhase::InAir, DoubleJumpStartMontageLenght - 0.3f);
	}
	else if (NewPhase == EJumpPhase::Landed)
	{
		PlayMontage(JumpLandedMontage);
	}
}

bool UAC_HeroJumpHandler::SetPhase(EJumpPhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return false;
	}

	CurrentPhase = NewPhase;

	return true;
}

float UAC_HeroJumpHandler::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (!HeroAnimInstance || !MontageToPlay) 
	{
		return 0.0f;
	}
	
	if(HeroASC->HasAnyMatchingGameplayTags(JumpMontageBlockedTags))
	{
		return 0.0f;
	}

	return HeroAnimInstance->Montage_Play(MontageToPlay);
}

void UAC_HeroJumpHandler::OnLanded(const FHitResult& Hit)
{
	JumpCount = 0;

	if (HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Shadow))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnLandedwithshadow!"));
		SetPhase(EJumpPhase::Landed);
	}
	else
	{
		SetPhaseAndPlayMontage(EJumpPhase::Landed);
	}
	
	SetPhaseAndPlayMontageWithDelay(EJumpPhase::None, 0.3f);
}

void UAC_HeroJumpHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}