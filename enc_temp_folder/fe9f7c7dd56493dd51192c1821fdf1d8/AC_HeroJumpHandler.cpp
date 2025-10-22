// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroJumpHandler.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (!HeroBase || !HeroMovement) 
	{
		return;
	}

	bool bCanJump = HeroMovement->IsMovingOnGround() || JumpCount < MaxJumpCount;
	if (!bCanJump) 
	{
		return;
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
	float JumpVelocity = FMath::Sqrt(2.0f * FMath::Abs(HeroMovement->GetGravityZ()) * JumpHeight);
	HeroBase->LaunchCharacter(FVector(0, 0, JumpVelocity), false, true);
}

bool UAC_HeroJumpHandler::IsInAir() const
{
	return HeroMovement && HeroMovement->IsFalling();
}

void UAC_HeroJumpHandler::SetPhaseAndPlayWithDelay(EJumpPhase NewPhase, float Delay)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPhase]()
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

