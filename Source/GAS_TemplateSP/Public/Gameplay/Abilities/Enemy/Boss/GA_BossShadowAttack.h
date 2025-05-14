// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Boss/GA_BossTargetBase.h"
#include "GA_BossShadowAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossShadowAttackExecute, const FGAS_TargetActorData&, Data);

UCLASS()
class GAS_TEMPLATESP_API UGA_BossShadowAttack : public UGA_BossTargetBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector) override;

	void ExecuteMovementChain();

	void OnTimerConfirm();

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	virtual void OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData) override;

	float GetTargetDistance(AActor* ShadowTargetActor);

	UPROPERTY(BlueprintAssignable)
	FOnBossShadowAttackExecute OnBossShadowAttackCompleted;

	UPROPERTY(EditDefaultsOnly)
	float ExecuteTime = 0.5f;
};
