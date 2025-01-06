// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_MontageAbility.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_MontageAbility : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	void CreatePlayMontageWaitForEvent();

	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	FName SectionName = NAME_None;

	/** Change to play the montage faster or slower */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	float PlayRate = 1.f;

	/** Any gameplay events matching this tag will activate the OnEventReceived callback and apply the gameplay effect containers for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	FGameplayTagContainer WaitForEventTag;

	/** If we need to turn off characters rotation during the gameplay effect */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	bool bTurnOffRotation;

	/** If we want that montage doesen't stop after ability end */
	UPROPERTY(EditDefaultsOnly, Category = "MontageAbility")
	bool bStopWhenAbilityEnds = true;

protected:
	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};
