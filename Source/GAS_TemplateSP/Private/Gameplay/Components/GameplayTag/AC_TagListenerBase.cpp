// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"

UAC_TagListenerBase::UAC_TagListenerBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_TagListenerBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGAS_CharacterBase>(GetOwner());
	if (!OwnerCharacter) 
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is null in %s"), *this->GetName());
		return;
	}

	OwnerCharacterASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!OwnerCharacterASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacterASC is null in %s"), *this->GetName());
		return;
	}

	OwnerCharacterMoveComp = OwnerCharacter->GetCharacterMovement();
	if (!OwnerCharacterMoveComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacterMoveComp is null in %s"), *this->GetName());
		return;
	}

	OwnerTagDelegatesComp = OwnerCharacter->GetTagDelegatesComponent();
	if (!OwnerTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerTagDelegatesComp is null in %s"), *this->GetName());
		return;
	}

	BindTagDelegates();
}

void UAC_TagListenerBase::BindTagDelegates()
{
	// Logic will be implemented in child classes
}






