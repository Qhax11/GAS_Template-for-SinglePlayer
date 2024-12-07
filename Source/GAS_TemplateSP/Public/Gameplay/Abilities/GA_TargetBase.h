// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GA_TargetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_TargetBase : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	bool BindInputForTargeting();

	virtual AGameplayAbilityTargetActor* SpawnAndSetupTargetActor();

	/** The TargetActor that we spawned */
	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType = EGameplayTargetingConfirmation::Custom;

private:

	UFUNCTION()
	void OnGameplayEventValidData(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnGameplayEventCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void ForceConfirmTargeting();

	UFUNCTION()
	void CancelAbility();

	UAbilityTask_WaitTargetData* WaitTargetData;
};
