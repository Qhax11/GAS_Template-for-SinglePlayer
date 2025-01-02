// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GA_TargetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_TargetBase : public UGA_MeleeAttackBase
{
	GENERATED_BODY()
	
public:

	UGA_TargetBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	bool BindInputForConfirmAndCancel();

	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetBase")
	bool bActorWillSpawnWithEQS = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetBase", meta = (EditCondition = "bActorWillSpawnWithEQS"))
	UEnvQuery* EQSQueryTemplate;

	virtual void StartEQSForTargetActorSpawnLocation();

	virtual void OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	/** The TargetActor class that we spawned */
	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TSubclassOf<AGAS_TargetActorBase> TargetActorClass;

protected:

	UFUNCTION()
	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION()
	virtual void OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION()
	void ConfirmTargetingFromInput();

	UFUNCTION()
	void CancelAbilityFromInput();

protected:

	/** The TargetActor that we spawned */
	TObjectPtr<AGAS_TargetActorBase> TargetActor;

};
