// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/Shadows/BossShadowTargetActor.h"


void ABossShadowTargetActor::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	Super::OnEnemyDetectionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	if (OtherActor == InstigatorCharacter)
	{
		return;
	}

	bIsTargetInRange = true;

	CurrentTarget = OtherActor;

	// If the direction hasn't changed, we play the montage manually. This is for first time detection.
	if (!UpdateRelativeDirectionToTarget())
	{
		PlayMontageWithCallback(AttackMontage);
	}
}

void ABossShadowTargetActor::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnEnemyDetectionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	UAbilitySystemComponent* OtherActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (!OtherActorASC)
	{
		return;
	}

	if (OtherActor == InstigatorCharacter)
	{
		return;
	}

	if (CurrentTarget = OtherActor)
	{
		CurrentTarget = nullptr;
	}

	bIsTargetInRange = false;
	Cancel();
}
