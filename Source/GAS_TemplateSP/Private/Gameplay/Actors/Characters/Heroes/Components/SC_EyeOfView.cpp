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
	
	bool bIsHeroTargeting = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting);
	if(bIsHeroTargeting)
	{
		FVector2D MouseInput;
		PC->GetInputMouseDelta(MouseInput.X, MouseInput.Y);

		if (!MouseInput.IsNearlyZero())
		{
			UpdateRotationFromMouseInput(MouseInput);
		}
	}
	else 
	{
		FVector Location;
		FRotator Rotation;
		PC->GetPlayerViewPoint(Location, Rotation);
		SetWorldRotation(Rotation);
	}

}

void USC_EyeOfView::UpdateRotationFromMouseInput(const FVector2D& MouseInput)
{
	// 2.5f is the actual sensitivity value for the player camera.
	FRotator DeltaRot;
	DeltaRot.Pitch = MouseInput.Y * 2.5f; 
	DeltaRot.Yaw = MouseInput.X * 2.5f; 
	DeltaRot.Roll = 0.0f;                          

	FRotator CurrentRotation = GetComponentRotation();

	FRotator NewRotation = CurrentRotation + DeltaRot;

	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -89.9f, 89.9f);

	SetWorldRotation(NewRotation);
}

FVector USC_EyeOfView::CalculateHologramTargetActorLocation(bool bDrawDebug)
{
	if (!TraceDistanceCurve) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceDistanceCurve is null in: %s"), *GetName());
		return FVector::ZeroVector;
	}
	TraceDistance = TraceDistanceCurve->GetFloatValue(GetComponentRotation().Pitch);

	FVector FirstTraceStart = GetComponentLocation();
	FVector ForwardVector = GetForwardVector();

	FVector TraceOffset = FVector(ForwardVector.X * TraceDistance, ForwardVector.Y * TraceDistance, ForwardVector.Z);
	FVector FirstTraceEnd = FirstTraceStart + TraceOffset;

	FHitResult HitResult; 
	FCollisionQueryParams CollisionParams;

	GetWorld()->LineTraceSingleByChannel(HitResult, FirstTraceStart, FirstTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug) 
	{
		DrawDebugLine(GetWorld(), FirstTraceStart, FirstTraceEnd, FColor::Blue, false, 0.0f, 0, 1.0f);
	}

	FVector SecondTraceStart = HitResult.TraceEnd;
	FVector SecondTraceEnd = HitResult.TraceEnd;
	SecondTraceEnd.Z *= -1.0f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SecondTraceStart, SecondTraceEnd, ECC_Visibility, CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), SecondTraceStart, SecondTraceEnd, FColor::Green, false, 0.0f, 0, 1.0f);
	}
	if (bDrawDebug && bHit)
	{
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.0f);  
	}

	return HitResult.ImpactPoint;
}

