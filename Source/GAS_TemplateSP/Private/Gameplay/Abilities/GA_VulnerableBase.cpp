// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_VulnerableBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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