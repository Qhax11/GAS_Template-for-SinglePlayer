// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HologramTargetActorBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AHologramTargetActorBase::AHologramTargetActorBase()
{
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCylinder"));
    SetRootComponent(CapsuleComponent);

    CapsuleComponent->SetCapsuleHalfHeight(96.0f);
    CapsuleComponent->SetCapsuleRadius(42.0f);

    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(CapsuleComponent);

    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHologramTargetActorBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateRelativeDirectionToTarget();
}

void AHologramTargetActorBase::UpdateRelativeDirectionToTarget()
{
    if (!CurrentTarget)
    {
        return;
    }

    FVector HologramLocation = GetActorLocation();
    FVector TargetLocation = CurrentTarget->GetActorLocation();

    FVector DirectionToTarget = (TargetLocation - HologramLocation).GetSafeNormal();

    // Calculate Dot Product with forward, right, and up vectors based on the target's position
    FVector ForwardVector = FVector(1, 0, 0);  // Assuming forward is along the X-axis for simplicity
    FVector RightVector = FVector(0, 1, 0);    // Assuming right is along the Y-axis for simplicity
    FVector UpVector = FVector(0, 0, 1);       // Assuming up is along the Z-axis for simplicity

    float ForwardDot = FVector::DotProduct(ForwardVector, DirectionToTarget);
    float RightDot = FVector::DotProduct(RightVector, DirectionToTarget);

    float Threshold = 0.5f;

    EHologramDirectionToTarget NewDirection = EHologramDirectionToTarget::HDT_None;

    if (FMath::Abs(ForwardDot) > Threshold)
    {
        NewDirection = (ForwardDot > 0.0f) ? EHologramDirectionToTarget::HDT_Forward : EHologramDirectionToTarget::HDT_Backward;
    }
    else if (FMath::Abs(RightDot) > Threshold)
    {
        NewDirection = (RightDot > 0.0f) ? EHologramDirectionToTarget::HDT_Right : EHologramDirectionToTarget::HDT_Left;
    }

    if (NewDirection != LastDirectionToTarget)
    {
        OnDirectionChanged(NewDirection);
        LastDirectionToTarget = NewDirection;
    }
}

UAnimMontage* AHologramTargetActorBase::GetAttackMontageFromRelativePositionToTarget()
{
    UAnimMontage** MontagePtr = DirectionalAttackMontage.Find(LastDirectionToTarget);

    if (MontagePtr)
    {
        return *MontagePtr;  
    }

    return nullptr;
}

void AHologramTargetActorBase::OnDirectionChanged(EHologramDirectionToTarget NewDirection)
{
    AttackMontage = GetAttackMontageFromRelativePositionToTarget();
}
