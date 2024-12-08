// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HologramTargetActorBase.h"

void AHologramTargetActorBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void AHologramTargetActorBase::ConfirmTargetingAndContinue()
{
	// We need hologram ref in ability so we give the self ref.
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	OverlapedActors.Add(this);
	FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
	TargetDataReadyDelegate.Broadcast(TargetData);

	// Its doing brodcast to task and its destroy the actor why idk?
	//Super::ConfirmTargetingAndContinue();
}
