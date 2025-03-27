// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_MovementListener.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAC_MovementListener::UAC_MovementListener()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_MovementListener::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is null in: %s"), *GetName());
    }

    OwnerMovementComp = OwnerCharacter->GetCharacterMovement();
    if (OwnerMovementComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerMovementComp is null in: %s"), *GetName());
    }

    MotionHistory.Empty();
}

void UAC_MovementListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwnerMovementComp || OwnerMovementComp->Velocity.SizeSquared() <= 0.0f)
    {
        return;
    }

    LastMovementTime = GetWorld()->GetTimeSeconds();
    TimeSinceLastSample += DeltaTime;

    if (TimeSinceLastSample >= SampleInterval)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        FVector CurrentLocation = OwnerCharacter->GetActorLocation();

        MotionHistory.Add(FMotionSample(CurrentLocation, CurrentTime));
        TimeSinceLastSample = 0.0f;

        // Eski sample'larý sil
        while (MotionHistory.Num() > 0 && (CurrentTime - MotionHistory[0].Time) > MaxHistoryTime)
        {
            MotionHistory.RemoveAt(0);
        }
    }
}

bool UAC_MovementListener::HasMovedInLastSeconds(float Seconds) const
{
    return (GetWorld()->GetTimeSeconds() - LastMovementTime) <= Seconds;
}

float UAC_MovementListener::GetDisplacementInLastSeconds(float Seconds) const
{
    if (OwnerCharacter)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        float TargetTime = CurrentTime - Seconds;

        for (const FMotionSample& Sample : MotionHistory)
        {
            if (Sample.Time >= TargetTime)
            {
                return FVector::Dist(Sample.Location, OwnerCharacter->GetActorLocation());
            }
        }
        return 0.0f;
    }
    else
    {
        return 0.0f;
    }
}

ERelativeDirection UAC_MovementListener::GetRelativeMovementDirection(float Seconds, AActor* ReferenceActor) const
{
    if (!ReferenceActor) return ERelativeDirection::None;

    FVector DisplacementDir = GetDisplacementDirectionInLastSeconds(Seconds);
    if (DisplacementDir.IsNearlyZero()) return ERelativeDirection::None;

    FVector RefForward = ReferenceActor->GetActorForwardVector();
    FVector RefRight = ReferenceActor->GetActorRightVector();

    float ForwardDot = FVector::DotProduct(RefForward, DisplacementDir);
    float RightDot = FVector::DotProduct(RefRight, DisplacementDir);

    if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
    {
        return ForwardDot > 0 ? ERelativeDirection::Forward : ERelativeDirection::Backward;
    }
    else
    {
        return RightDot > 0 ? ERelativeDirection::Right : ERelativeDirection::Left;
    }
}

FVector UAC_MovementListener::GetDisplacementDirectionInLastSeconds(float Seconds) const
{
    if (OwnerCharacter)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        float TargetTime = CurrentTime - Seconds;

        for (const FMotionSample& Sample : MotionHistory)
        {
            if (Sample.Time >= TargetTime)
            {
                return (OwnerCharacter->GetActorLocation() - Sample.Location).GetSafeNormal();
            }
        }
        return FVector::ZeroVector;
    }
    else
    {
        return FVector::ZeroVector;
    }
}

