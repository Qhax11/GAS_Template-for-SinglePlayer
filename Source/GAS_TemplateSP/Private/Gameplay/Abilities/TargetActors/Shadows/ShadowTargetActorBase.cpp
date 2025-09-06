// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/Shadows/ShadowTargetActorBase.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AShadowTargetActorBase::AShadowTargetActorBase()
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
    EnemyDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AShadowTargetActorBase::OnEnemyDetectionBeginOverlap);
    EnemyDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AShadowTargetActorBase::OnEnemyDetectionEndOverlap);

    PrimaryActorTick.bCanEverTick = true;
}

void AShadowTargetActorBase::BeginPlay()
{
    Super::BeginPlay();

    InstigatorCharacter = Cast<AGAS_CharacterBase>(GetInstigator());
    if (!InstigatorCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("InstigatorCharacter is null in: %s"), *GetName());
        return;
    }

    AnimInstance = SkeletalMesh->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null in: %s"), *GetName());
        return;
    }

    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AShadowTargetActorBase::OnPlayMontageNotify);
}

void AShadowTargetActorBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateRelativeDirectionToTarget();
    RotateToTarget(CurrentTarget, DeltaSeconds);
}

void AShadowTargetActorBase::Confirm()
{
    OnConfirm.Broadcast(FGAS_TargetActorData(SelectedAttackAbilityClass, SelectedShadowAbilityCDO, this));
}

void AShadowTargetActorBase::Cancel()
{
    OnCancel.Broadcast(FGAS_TargetActorData(SelectedAttackAbilityClass, SelectedShadowAbilityCDO, this));
}

void AShadowTargetActorBase::RotateToTarget(AActor* TargetActor, float DeltaTime)
{
    if (!TargetActor)
    {
        return;
    }

    FVector CurrentLocation = GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();

    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

    SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll));
}

bool AShadowTargetActorBase::UpdateRelativeDirectionToTarget()
{
    if (!CurrentTarget)
    {
        return false;
    }

    FVector ShadowLocation = GetActorLocation();
    FVector TargetLocation = CurrentTarget->GetActorLocation();

    FVector DirectionToTarget = (TargetLocation - ShadowLocation).GetSafeNormal();

    // Calculate Dot Product with forward, right, and up vectors based on the target's position
    FVector ForwardVector = FVector(1, 0, 0);  // Assuming forward is along the X-axis for simplicity
    FVector RightVector = FVector(0, 1, 0);    // Assuming right is along the Y-axis for simplicity
    FVector UpVector = FVector(0, 0, 1);       // Assuming up is along the Z-axis for simplicity

    float ForwardDot = FVector::DotProduct(ForwardVector, DirectionToTarget);
    float RightDot = FVector::DotProduct(RightVector, DirectionToTarget);

    float Threshold = 0.5f;

    EShadowDirectionToTarget NewDirection = EShadowDirectionToTarget::HDT_None;

    if (FMath::Abs(ForwardDot) > Threshold)
    {
        NewDirection = (ForwardDot > 0.0f) ? EShadowDirectionToTarget::HDT_Forward : EShadowDirectionToTarget::HDT_Backward;
    }
    else if (FMath::Abs(RightDot) > Threshold)
    {
        NewDirection = (RightDot > 0.0f) ? EShadowDirectionToTarget::HDT_Right : EShadowDirectionToTarget::HDT_Left;
    }

    if (NewDirection != LastDirectionToTarget)
    {
        LastDirectionToTarget = NewDirection;
        OnDirectionToTargetChanged(NewDirection);
        return true;
    }

    return false;
}

void AShadowTargetActorBase::SetCurrentTarget(AActor* NewCurrentTarget)
{
    if (NewCurrentTarget) 
    {
        CurrentTarget = NewCurrentTarget;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Current Target couln't set in: %s"), *GetName());
    }
}

AActor* AShadowTargetActorBase::GetCurrentTarget()
{
    if (CurrentTarget) 
    {
        return CurrentTarget;
    }
    else
    {
        return nullptr;
    }
}

TSubclassOf<UGA_MeleeAttackBase> AShadowTargetActorBase::GetSelectedAttackAbilityClass()
{
    if (SelectedAttackAbilityClass)
    {
        return SelectedAttackAbilityClass;
    }

    return nullptr;
}

void AShadowTargetActorBase::OnDirectionToTargetChanged(EShadowDirectionToTarget NewDirection)
{
    UptadeAttackAbilityClassAndMontageFromRelativePositionToTarget();
    PlayMontageWithCallback(AttackMontage);
}

void AShadowTargetActorBase::UptadeAttackAbilityClassAndMontageFromRelativePositionToTarget()
{
    if (GetAttackAbilityFromRelativePositionToTarget())
    {
        SelectedAttackAbilityClass = GetAttackAbilityFromRelativePositionToTarget();
        if (UGA_MeleeAttackBase* NewMeleeAttack = Cast<UGA_MeleeAttackBase>(SelectedAttackAbilityClass->GetDefaultObject()))
        {
            SelectedShadowAbilityCDO = NewMeleeAttack;
            AttackMontage = NewMeleeAttack->AnimMontage;
        }
    }
}

TSubclassOf<UGA_MeleeAttackBase> AShadowTargetActorBase::GetAttackAbilityFromRelativePositionToTarget()
{
    if (const TSubclassOf<UGA_MeleeAttackBase>* FoundAbility = DirectionalAttackAbilities.Find(LastDirectionToTarget))
    {
        return *FoundAbility;
    }

    return nullptr;
}

void AShadowTargetActorBase::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
    if (!OtherActorASC)
    {
        return;
    }

    if (OtherActor == InstigatorCharacter)
    {
        return;
    }
}

void AShadowTargetActorBase::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
    if (!OtherActorASC)
    {
        return;
    }

    if (OtherActor == InstigatorCharacter)
    {
        return;
    }
}

void AShadowTargetActorBase::PlayMontageWithCallback(UAnimMontage* MontageToPlay)
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

void AShadowTargetActorBase::OnPlayMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == FName(TEXT("FirstSectionFinished"))) 
    {
        SkeletalMesh->bPauseAnims = true;
    }
}

void AShadowTargetActorBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}
