// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/AnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "KismetAnimationLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAnimInstanceBase::UAnimInstanceBase()
{
	Speed = 0.f;
	bIsInAir = false;
	bIsAccelerating = false;
}

void UAnimInstanceBase::NativeInitializeAnimation()
{
	OwnerCharacterBase = Cast<AGAS_CharacterBase>(TryGetPawnOwner());
	if (OwnerCharacterBase)
	{
		CharacterMovement = OwnerCharacterBase->GetCharacterMovement();
		OwnerASC = OwnerCharacterBase->GetAbilitySystemComponent();
	}
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerCharacterBase || !CharacterMovement)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterRef or CharacterMovement is null in: %s"), *GetName());
		return;
	}

	// Only update speed if the character is not dashing.
    // This prevents sudden speed drops during dash animations, which could negatively affect animation blending.
	if (!DoesOwnerHaveTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash)) 
	{
		FVector Velocity = OwnerCharacterBase->GetVelocity();
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}

	LocomationDirection = UKismetAnimationLibrary::CalculateDirection(OwnerCharacterBase->GetVelocity(), OwnerCharacterBase->GetActorRotation());

	bIsInAir = CharacterMovement->IsFalling();

	if (CharacterMovement->GetCurrentAcceleration().Size() > 0.f)
	{
		bIsAccelerating = true;
	}
	else
	{
		bIsAccelerating = false;
	}
}

bool UAnimInstanceBase::DoesOwnerHaveTag(const FGameplayTag GameplayTag) const
{
	if (OwnerASC)
	{
		return OwnerASC->HasMatchingGameplayTag(GameplayTag);
	}

	return false;
}


