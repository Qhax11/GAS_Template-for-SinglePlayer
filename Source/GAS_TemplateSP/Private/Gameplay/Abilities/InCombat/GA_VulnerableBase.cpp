// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/GA_VulnerableBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_VulnerableBase::UGA_VulnerableBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead);
}

void UGA_VulnerableBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	UCharacterMovementComponent* MovementComp = AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComp)
	{
		MovementComp->StopMovementImmediately();
	}
	
}