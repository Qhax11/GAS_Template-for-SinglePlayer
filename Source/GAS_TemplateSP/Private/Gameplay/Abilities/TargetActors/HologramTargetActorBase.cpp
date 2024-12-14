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
