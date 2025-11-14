// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"

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

	if (UAC_TagDelegates* TagDelegatesComponent = GetOwner()->GetComponentByClass<UAC_TagDelegates>())
	{
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnPatrollingTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnPatrollingTagRemoved);

		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnStrafingTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnStrafingTagRemoved);

		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnRunningTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnRunningTagRemoved);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s, cannot listen tags."), *this->GetName());
		return;
	}
}

void UAC_TagListenerBase::OnPatrollingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}

void UAC_TagListenerBase::OnPatrollingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = false;
}

void UAC_TagListenerBase::OnStrafingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = false;
}

void UAC_TagListenerBase::OnStrafingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}

void UAC_TagListenerBase::OnRunningTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (OwnerCharacterASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		OwnerCharacterMoveComp->bOrientRotationToMovement = true;
	}
}

void UAC_TagListenerBase::OnRunningTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (OwnerCharacterASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		OwnerCharacterMoveComp->bOrientRotationToMovement = false;
	}
}

