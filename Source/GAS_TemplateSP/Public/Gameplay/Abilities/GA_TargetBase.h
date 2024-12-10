// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "EnhancedInputComponent.h"
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

	bool BindInputForTargeting();

	virtual AGameplayAbilityTargetActor* SpawnAndSetupTargetActor();

	/** The TargetActor that we spawned */
	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType = EGameplayTargetingConfirmation::Custom;

	TObjectPtr<UAbilityTask_WaitTargetData> WaitTargetData;
	//TWeakObjectPtr<UAbilityTask_WaitTargetData> WaitTargetData;
protected:

	UFUNCTION()
	virtual void OnGameplayEventValidData(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	virtual void OnGameplayEventCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void ConfirmTargetingFromInput(const FInputActionValue& Value);

	UFUNCTION()
	void CancelAbilityFromInput(const FInputActionValue& Value);

};
