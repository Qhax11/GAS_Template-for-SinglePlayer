// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HologramTargetActorBase.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

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

    EnemyDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    EnemyDetectionSphere->SetupAttachment(CapsuleComponent);
    EnemyDetectionSphere->InitSphereRadius(300.0f);
    EnemyDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EnemyDetectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    EnemyDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    EnemyDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHologramTargetActorBase::OnEnemyDetectionBeginOverlap);
    EnemyDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AHologramTargetActorBase::OnEnemyDetectionEndOverlap);
}

void AHologramTargetActorBase::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = SkeletalMesh->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null in: %s"), *GetName());
    }

    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AHologramTargetActorBase::OnPlayMontageNotify);
}

void AHologramTargetActorBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateRelativeDirectionToTarget();
}

bool AHologramTargetActorBase::UpdateRelativeDirectionToTarget()
{
    if (!CurrentTarget)
    {
        return false;
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
        LastDirectionToTarget = NewDirection;
        OnDirectionToTargetChanged(NewDirection);
        return true;
    }

    return false;
}

void AHologramTargetActorBase::OnDirectionToTargetChanged(EHologramDirectionToTarget NewDirection)
{
    UptadeAttackMontageFromRelativePositionToTarget();
    PlayMontageWithCallback(AttackMontage);
}

void AHologramTargetActorBase::UptadeAttackMontageFromRelativePositionToTarget()
{
    AttackMontage = GetAttackMontageFromRelativePositionToTarget();
}

UAnimMontage* AHologramTargetActorBase::GetAttackMontageFromRelativePositionToTarget()
{
    UAnimMontage** MontagePtr = DirectionalAttackMontages.Find(LastDirectionToTarget);

    if (MontagePtr)
    {
        return *MontagePtr;
    }

    return nullptr;
}

void AHologramTargetActorBase::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
    if (!OtherActorASC)
    {
        return;
    }

}

void AHologramTargetActorBase::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
    if (!OtherActorASC)
    {
        return;
    }

}

void AHologramTargetActorBase::PlayMontageWithCallback(UAnimMontage* MontageToPlay)
{
    if (!bIsTargetInRange) 
    {
        return;
    }

    if (!MontageToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("No montage provided to play."));
        return;
    }

    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null in: %s "), *GetName());
        return;
    }

    if (SkeletalMesh->bPauseAnims) 
    {
        SkeletalMesh->bPauseAnims = false;
    }

    AnimInstance->Montage_Play(MontageToPlay);
}

void AHologramTargetActorBase::OnPlayMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == FName(TEXT("FirstSectionFinished"))) 
    {
        SkeletalMesh->bPauseAnims = true;
    }
}

void AHologramTargetActorBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}
