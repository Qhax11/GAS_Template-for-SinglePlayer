// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/Listener/AC_EnemyTagListener.h"

void UAC_EnemyTagListener::BindTagDelegates()
{
	if (!OwnerTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s"), *this->GetName());
		return;
	}

	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling, EListenMode::OnAdded).BindDynamic(this, &UAC_EnemyTagListener::OnPatrollingTagAdded);
	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling, EListenMode::OnRemoved).BindDynamic(this, &UAC_EnemyTagListener::OnPatrollingTagRemoved);

	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnAdded).BindDynamic(this, &UAC_EnemyTagListener::OnStrafingTagAdded);
	OwnerTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing, EListenMode::OnRemoved).BindDynamic(this, &UAC_EnemyTagListener::OnStrafingTagRemoved);
}

void UAC_EnemyTagListener::OnPatrollingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}

void UAC_EnemyTagListener::OnPatrollingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = false;
}

void UAC_EnemyTagListener::OnStrafingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = false;
}

void UAC_EnemyTagListener::OnStrafingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerCharacterMoveComp->bOrientRotationToMovement = true;
}
