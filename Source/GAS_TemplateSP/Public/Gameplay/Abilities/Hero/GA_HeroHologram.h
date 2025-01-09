// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TargetBase.h"
#include "GA_HeroHologram.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_HeroHologram : public UGA_TargetBase
{
	GENERATED_BODY()

protected:
	UGA_HeroHologram();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
	virtual void SpawnAndSetupTargetActor(FRotator Rotation, FVector Location) override;

	void SetHologramToHologramController();

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	virtual void OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result) override;
public:


};
