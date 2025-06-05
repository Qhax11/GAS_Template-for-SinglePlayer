// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GA_TargetBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_TargetBase : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UGA_TargetBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetBase|EQS")
	bool bActorWillSpawnWithEQS = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetBase|EQS", meta = (EditCondition = "bActorWillSpawnWithEQS"))
	UEnvQuery* EQSQueryTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetBase|EQS")
	TEnumAsByte<EEnvQueryRunMode::Type> QueryRunMode = EEnvQueryRunMode::RandomBest25Pct;

	virtual void StartEQSForTargetActorSpawnLocation();

	virtual void OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector);

	UFUNCTION()
	virtual void OnTargetActorInitialized();

	/** The TargetActor class that we spawned */
	UPROPERTY(EditDefaultsOnly, Category = "TargetBase")
	TSubclassOf<AGAS_TargetActorBase> TargetActorClass;

	UPROPERTY(EditAnywhere, Category = "UGA_TargetBase")
	TArray<TSubclassOf<UGameplayEffect>> AbilityConfirmEffects;

protected:

	UFUNCTION()
	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION()
	virtual void OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData);

protected:

	/** The TargetActor that we spawned */
	TObjectPtr<AGAS_TargetActorBase> TargetActor;

};
