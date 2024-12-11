// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "GA_TargetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_TargetBase : public UGA_MontageAbility
{
	GENERATED_BODY()
	
public:

	UGA_TargetBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	bool BindInputForConfirmAndCancel();

	virtual AGAS_TargetActorBase* SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector);

	/** The TargetActor class that we spawned */
	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TSubclassOf<AGAS_TargetActorBase> TargetActorClass;

protected:

	UFUNCTION()
	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION()
	virtual void OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION()
	void ConfirmTargetingFromInput();

	UFUNCTION()
	void CancelAbilityFromInput();

private:

	/** The TargetActor that we spawned */
	TObjectPtr<AGAS_TargetActorBase> TargetActor;

};
