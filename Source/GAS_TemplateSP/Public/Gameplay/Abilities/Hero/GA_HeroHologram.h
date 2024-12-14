// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/GA_TargetBase.h"
#include "GA_HeroHologram.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_HeroHologram : public UGA_TargetBase
{
	GENERATED_BODY()

private:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;
	
	virtual AGAS_TargetActorBase* SpawnAndSetupTargetActor(FRotator Rotation, FVector Location) override;

};
