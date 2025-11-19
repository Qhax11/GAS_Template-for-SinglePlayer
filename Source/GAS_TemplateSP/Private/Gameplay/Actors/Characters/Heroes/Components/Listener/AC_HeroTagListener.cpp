// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroTagListener.h"

void UAC_HeroTagListener::BindTagDelegates()
{
	if (!OwnerTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s"), *this->GetName());
		return;
	}

	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroTagListener::OnRunningTagAdded);
	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Running, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroTagListener::OnRunningTagRemoved);

	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked, EListenMode::OnAdded).BindDynamic(this, &UAC_HeroTagListener::OnHeroTargetLockedTagAdded);
	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroTagListener::OnHeroTargetLockedTagRemoved);
}

void UAC_HeroTagListener::OnRunningTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (OwnerCharacterASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		//OwnerCharacterMoveComp->bOrientRotationToMovement = true;
	}
}

void UAC_HeroTagListener::OnRunningTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (OwnerCharacterASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
	{
		//OwnerCharacterMoveComp->bOrientRotationToMovement = false;
	}
}

void UAC_HeroTagListener::OnHeroTargetLockedTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (!OwnerCharacterASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Moving_Running))
	{
		//OwnerCharacterMoveComp->bOrientRotationToMovement = false;
	}
}

void UAC_HeroTagListener::OnHeroTargetLockedTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	//OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}
