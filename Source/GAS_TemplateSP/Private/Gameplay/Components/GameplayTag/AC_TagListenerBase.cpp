// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UAC_TagListenerBase::UAC_TagListenerBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_TagListenerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacterMoveComp = Character->GetCharacterMovement();
		if (!OwnerCharacterMoveComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("OwnerCharacterMoveComp is null in %s"), *this->GetName());
			return;
		}
	}

	if (UAC_TagDelegates* TagDelegatesComponent = GetOwner()->GetComponentByClass<UAC_TagDelegates>())
	{
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnStrafingTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnStrafingTagRemoved);

		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Walking, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnWalkingTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Walking, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnWalkingTagRemoved);

		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Jogging, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnJoggingTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Jogging, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnJoggingTagRemoved);

		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnAdded).BindDynamic(this, &UAC_TagListenerBase::OnRunningTagAdded);
		TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagListenerBase::OnRunningTagRemoved);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s, cannot listen tags."), *this->GetName());
		return;
	}
}

void UAC_TagListenerBase::OnStrafingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = false;
}

void UAC_TagListenerBase::OnStrafingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}

void UAC_TagListenerBase::OnWalkingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->MaxWalkSpeed = WalkingSpeed;
}

void UAC_TagListenerBase::OnWalkingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}

void UAC_TagListenerBase::OnJoggingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->MaxWalkSpeed = JoggingSpeed;
}

void UAC_TagListenerBase::OnJoggingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}

void UAC_TagListenerBase::OnRunningTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->MaxWalkSpeed = RunningSpeed;
}

void UAC_TagListenerBase::OnRunningTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}



