// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AbilitySystemComponent.h"


void AHologramTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(OwningAbility->GetAvatarActorFromActorInfo());
	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
	}

}

void AHologramTargetActor::ConfirmTargetingAndContinue()
{
	// We need hologram ref in ability so we give the self ref.
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	OverlapedActors.Add(this);
	FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
	TargetDataReadyDelegate.Broadcast(TargetData);

	// Its doing brodcast to task and its destroy the actor why idk?
	//Super::ConfirmTargetingAndContinue();
}
