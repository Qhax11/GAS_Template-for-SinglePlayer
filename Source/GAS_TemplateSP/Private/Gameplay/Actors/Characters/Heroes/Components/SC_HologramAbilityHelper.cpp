// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/SC_HologramAbilityHelper.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Tags/GAS_Tags.h"

USC_HologramAbilityHelper::USC_HologramAbilityHelper()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USC_HologramAbilityHelper::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is null in: %s"), *GetName());
		return;
	}

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	UAC_TargetLockSystem* TargetLockSystem = HeroBase->GetTargetLockSystemComponent();
	if (!TargetLockSystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLockSystem is null in: %s"), *GetName());
		return;
	}
	TargetLockSystem->OnStartTargetLock.AddDynamic(this, &USC_HologramAbilityHelper::OnStartTargetLock);
	TargetLockSystem->OnHeroRotationToTargetCompleted.AddDynamic(this, &USC_HologramAbilityHelper::OnHeroRotationToTargetCompleted);
	TargetLockSystem->OnEndTargetLock.AddDynamic(this, &USC_HologramAbilityHelper::OnEndTargetLock);
}

void USC_HologramAbilityHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HeroBase || !PC)
	{
		return;
	}
	
	bool bIsHeroTargetLocked = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
	bool bIsHeroHologramAbilityTargeting = HeroBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting_Hologram);

	if(bIsHeroTargetLocked)
	{
		UpdateYawToActLikeRelative();
		FVector2D MouseInput;
		PC->GetInputMouseDelta(MouseInput.X, MouseInput.Y);
	
		if (!MouseInput.IsNearlyZero())
		{
		}
		UpdateRotationFromMouseInput(MouseInput);

	}
	else 
	{
		FVector Location;
		FRotator Rotation;
		PC->GetPlayerViewPoint(Location, Rotation);
		SetWorldRotation(Rotation);
	}

}

void USC_HologramAbilityHelper::UpdateRotationFromMouseInput(const FVector2D& MouseInput)
{
	// 2.5f is the actual sensitivity value for the player camera.
	FRotator DeltaRot;
	DeltaRot.Pitch = MouseInput.Y * 2.5f;
	DeltaRot.Yaw = MouseInput.X * 2.5f;
	DeltaRot.Roll = 0.0f;

	FRotator CurrentRotation = GetComponentRotation();

	FRotator NewRotation = CurrentRotation + DeltaRot;

	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -45.0f, 89.9f);

	// Yaw Clamp
	float HeroBaseYaw = HeroBase->GetActorRotation().Yaw;
	// Calculate the difference between the new yaw and character yaw
	float YawDifference = FMath::FindDeltaAngleDegrees(HeroBaseYaw, NewRotation.Yaw);
	// Clamp the yaw difference to the desired range, e.g., -90 to 90 degrees
	float ClampedYawDifference = FMath::Clamp(YawDifference, -90.0f, 90.0f);
	// Update NewRotation's yaw to respect the clamped difference
	NewRotation.Yaw = HeroBaseYaw + ClampedYawDifference;

	SetWorldRotation(NewRotation);
}

FVector USC_HologramAbilityHelper::CalculateHologramTargetActorLocation(bool bDrawDebug)
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

void USC_HologramAbilityHelper::OnStartTargetLock()
{
	//SetWorldRotation(FRotator::ZeroRotator);
	//SetUsingAbsoluteRotation(false);
}

void USC_HologramAbilityHelper::OnHeroRotationToTargetCompleted()
{
	//FRotator CurrentRotation = GetComponentRotation() - HeroBase->GetActorRotation();
	//SetRelativeRotation(CurrentRotation);
}

void USC_HologramAbilityHelper::OnEndTargetLock()
{

}

void USC_HologramAbilityHelper::UpdateYawToActLikeRelative()
{
	if (!HeroBase) // HeroBase kontrolü
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	// Karakterin dünya rotasyonunu al
	float HeroYaw = HeroBase->GetActorRotation().Yaw;

	// Component'in dünya rotasyonunu al
	FRotator ComponentWorldRotation = GetComponentRotation();

	// Component'in relative yaw'unu hesapla
	float RelativeYaw = FMath::FindDeltaAngleDegrees(HeroYaw, ComponentWorldRotation.Yaw);

	// Yeni relative yaw'u karakterin dünya rotasyonuna ekleyerek yeni dünya rotasyonunu oluþtur
	FRotator NewWorldRotation = FRotator(ComponentWorldRotation.Pitch, HeroYaw + RelativeYaw, ComponentWorldRotation.Roll);

	// Dünya rotasyonunu uygula
	SetWorldRotation(NewWorldRotation);

	UE_LOG(LogTemp, Log, TEXT("Updated Component Yaw to act relative: HeroYaw=%f, RelativeYaw=%f, FinalYaw=%f"),
		HeroYaw, RelativeYaw, NewWorldRotation.Yaw);
}
