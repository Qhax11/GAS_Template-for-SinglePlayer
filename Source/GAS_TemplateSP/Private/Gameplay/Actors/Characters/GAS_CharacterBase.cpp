// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/AC_AttributesListenerBase.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Components/AC_Team.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Components/GameplayTag/AC_TagDispatcher.h"
#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "Gameplay/Components/AC_PostureHandler.h"
#include "Gameplay/Components/AC_FootstepBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AGAS_CharacterBase::AGAS_CharacterBase(const class FObjectInitializer& ObjectInitializer)
{
	CharacterASC = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("CharacterASC"));

	AbilitySetComponent = CreateDefaultSubobject<UAC_AbilitySet>(TEXT("AbilitySetComponent"));

	TeamComponent = CreateDefaultSubobject<UAC_Team>(TEXT("TeamComponent"));

	TagDelegatesComponent = CreateDefaultSubobject<UAC_TagDelegates>(TEXT("TagDelegatesComponent"));

	TagDispatcherComponent = CreateDefaultSubobject<UAC_TagDispatcher>(TEXT("TagDispatcherComponent"));

	TagListenerComponent = CreateDefaultSubobject<UAC_TagListenerBase>(TEXT("TagListenerComponent"));

	AttributesListenerComponent = CreateDefaultSubobject<UAC_AttributesListenerBase>(TEXT("AttributesListenerComponent"));

	GameplayDataComponent = CreateDefaultSubobject<UAC_GameplayData>(TEXT("GameplayDataComponent"));

	PostureHandlerComponent = CreateDefaultSubobject<UAC_PostureHandler>(TEXT("PostureHandlerComponent"));

	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChildComponent"));

	FootstepComp = CreateDefaultSubobject<UAC_FootstepBase>(TEXT("FootstepComp"));

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
}

void AGAS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!CharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterASC doesen't exist in %s"), *this->GetName());
		return;
	}

	CharacterASC->InitAbilityActorInfo(this, this);
	AbilitySetComponent->Initialize(CharacterASC);

	if (WeaponChildComponent && GetMesh()->DoesSocketExist(WeaponSocketName))
	{
		WeaponChildComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon socket '%s' not found or WeaponChildComponent is null on %s"), *WeaponSocketName.ToString(), *GetName());
	}
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

void AGAS_CharacterBase::DisableCollision(ECollisionEnabled::Type NewType)
{
	// ECC_GameTraceChannel1 is Dead object collision, you can check ProjectSettings->Engine->Collision->CollisionObject
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent()) 
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		GetCapsuleComponent()->SetCollisionEnabled(NewType);
	}

	if (USkeletalMeshComponent* CharacterMesh = GetMesh()) 
	{
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		GetMesh()->SetCollisionEnabled(NewType);
	}
}

void AGAS_CharacterBase::EnableCollision()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AGAS_CharacterBase::DisableMesh()
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		GetMesh()->SetVisibility(false, true);
	}
}

void AGAS_CharacterBase::EnableMesh()
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		GetMesh()->SetVisibility(true, true);
	}
}

void AGAS_CharacterBase::AddGameplayTagIfNotExist(FGameplayTag GameplayTag)
{
	if (!CharacterASC) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterASC is null in: %s"), *GetName());
		return;
	}

	if (!CharacterASC->HasMatchingGameplayTag(GameplayTag)) 
	{
		CharacterASC->AddLooseGameplayTag(GameplayTag);
	}
}

void AGAS_CharacterBase::RemoveGameplayTagIfExist(FGameplayTag GameplayTag)
{
	if (!CharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterASC is null in: %s"), *GetName());
		return;
	}

	if (CharacterASC->HasMatchingGameplayTag(GameplayTag))
	{
		CharacterASC->RemoveLooseGameplayTag(GameplayTag, 100);
	}
}

void AGAS_CharacterBase::AddGameplayTagsIfNotExist(FGameplayTagContainer& GameplayTags)
{
	if (!CharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterASC is null in: %s"), *GetName());
		return;
	}

	CharacterASC->AddLooseGameplayTags(GameplayTags);
}

void AGAS_CharacterBase::RemoveGameplayTagsIfExist(FGameplayTagContainer& GameplayTags)
{
	if (!CharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterASC is null in: %s"), *GetName());
		return;
	}

	CharacterASC->RemoveLooseGameplayTags(GameplayTags);
}









