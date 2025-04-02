// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GA_EnemyStrafingBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyStrafingBase : public UGA_EnemyMovementBase
{
	GENERATED_BODY()

protected:
	UGA_EnemyStrafingBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStrafingBase|EQS")
	UEnvQuery* EQSQueryTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStrafingBase|EQS")
	TEnumAsByte<EEnvQueryRunMode::Type> QueryRunMode = EEnvQueryRunMode::RandomBest25Pct;

	virtual void StartEQSForStrafingLocation();

	virtual void OnStrafingLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
