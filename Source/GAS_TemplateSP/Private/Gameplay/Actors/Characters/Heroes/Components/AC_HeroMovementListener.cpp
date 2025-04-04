// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

UAC_HeroMovementListener::UAC_HeroMovementListener()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_HeroMovementListener::BeginPlay()
{
	Super::BeginPlay();

    OwnerHero = Cast<AGAS_HeroBase>(GetOwner());
    if (!OwnerHero)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerHero is null in: %s"), *GetName());
        return;
    }

    OwnerMovementComp = OwnerHero->GetCharacterMovement();
    if (!OwnerMovementComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerMovementComp is null in: %s"), *GetName());
        return;
    }

    MotionHistory.Empty();
}

void UAC_HeroMovementListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
        FVector CurrentLocation = OwnerHero->GetActorLocation();

        MotionHistory.Add(FMotionSample(CurrentLocation, CurrentTime));
        TimeSinceLastSample = 0.0f;

        // Eski sample'larý sil
        while (MotionHistory.Num() > 0 && (CurrentTime - MotionHistory[0].Time) > MaxHistoryTime)
        {
            MotionHistory.RemoveAt(0);
        }
    }
}

bool UAC_HeroMovementListener::HasMovedInLastSeconds(float Seconds) const
{
    return (GetWorld()->GetTimeSeconds() - LastMovementTime) <= Seconds;
}

float UAC_HeroMovementListener::GetDisplacementInLastSeconds(float Seconds) const
{
    if (OwnerHero)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        float TargetTime = CurrentTime - Seconds;

        for (const FMotionSample& Sample : MotionHistory)
        {
            if (Sample.Time >= TargetTime)
            {
                return FVector::Dist(Sample.Location, OwnerHero->GetActorLocation());
            }
        }
        return 0.0f;
    }
    else
    {
        return 0.0f;
    }
}

EHeroRelativeDirection UAC_HeroMovementListener::GetHeroLastMovementDirectionByLastInput() const
{
    if (!OwnerHero) return EHeroRelativeDirection::None;

    FVector2D Input = OwnerHero->GetHeroControlComponent()->LastMovementInput;

    if (Input.IsNearlyZero()) return EHeroRelativeDirection::None;


    // Y (Forward/Backward) yönü eþit veya daha baskýnsa onu önceliklendir
    if (FMath::Abs(Input.Y) >= FMath::Abs(Input.X))
    {
        return Input.Y > 0 ? EHeroRelativeDirection::Forward : EHeroRelativeDirection::Backward;
    }
    else
    {
        return Input.X > 0 ? EHeroRelativeDirection::Right : EHeroRelativeDirection::Left;
    }
}

FGameplayTag UAC_HeroMovementListener::GetHeroLastMovementDirectionTagByLastInput() const
{
    EHeroRelativeDirection Direction = GetHeroLastMovementDirectionByLastInput();
    switch (Direction)
    {
    case EHeroRelativeDirection::Forward:
        return GAS_Tags::TAG_AI_Direction_Resolved_Forward;
    case EHeroRelativeDirection::Backward:
        return GAS_Tags::TAG_AI_Direction_Resolved_Backward;
    case EHeroRelativeDirection::Left:
        return GAS_Tags::TAG_AI_Direction_Resolved_Left;
    case EHeroRelativeDirection::Right:
        return GAS_Tags::TAG_AI_Direction_Resolved_Right;
    default:
        return FGameplayTag();
    }
}




