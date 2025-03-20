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
}

void UAC_MovementListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
        if (MovementComp && MovementComp->Velocity.SizeSquared() > 0.0f)
        {
            LastMovementTime = GetWorld()->GetTimeSeconds();
        }
    }
}

bool UAC_MovementListener::HasMovedInLastSeconds(float Seconds) const
{
    return (GetWorld()->GetTimeSeconds() - LastMovementTime) <= Seconds;
}

