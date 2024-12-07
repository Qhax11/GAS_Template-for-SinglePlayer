// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/AC_AttributesListenerBase.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Components/AC_Team.h"
#include "Gameplay/Components/AC_TagDelegates.h"
#include "Gameplay/Components/AC_TagDispatcher.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AGAS_CharacterBase::AGAS_CharacterBase(const class FObjectInitializer& ObjectInitializer)
{
	CharacterASC = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("CharacterASC"));

	AbilitySetComponent = CreateDefaultSubobject<UAC_AbilitySet>(TEXT("AbilitySetComponent"));

	TeamComponent = CreateDefaultSubobject<UAC_Team>(TEXT("TeamComponent"));

	TagDelegatesComponent = CreateDefaultSubobject<UAC_TagDelegates>(TEXT("TagDelegatesComponent"));

	TagDispatcherComponent = CreateDefaultSubobject<UAC_TagDispatcher>(TEXT("TagDispatcherComponent"));

	AttributesListenerComponent = CreateDefaultSubobject<UAC_AttributesListenerBase>(TEXT("AttributesListenerComponent"));

	GameplayDataComponent = CreateDefaultSubobject<UAC_GameplayData>(TEXT("GameplayDataComponent"));
}

void AGAS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!CharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PaperCharacterASC doesen't exist in %s"), *this->GetName());
		return;
	}

	CharacterASC->InitAbilityActorInfo(this, this);
	AbilitySetComponent->Initialize(CharacterASC);
}

UAbilitySystemComponent* AGAS_CharacterBase::GetAbilitySystemComponent() const
{
	return CharacterASC;
}

void AGAS_CharacterBase::DisableMovement()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->DisableMovement();
	}
}

void AGAS_CharacterBase::EnableMovement()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AGAS_CharacterBase::DisableCollision()
{
	// Dead object collision, you can check ProjectSettings->Engine->Collision->CollisionObject
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent()) 
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	}
}

void AGAS_CharacterBase::EnableCollision()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	}
}





