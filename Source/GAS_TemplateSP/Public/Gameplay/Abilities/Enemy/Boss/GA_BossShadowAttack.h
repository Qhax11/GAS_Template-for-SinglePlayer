// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Boss/GA_BossTargetBase.h"
#include "GA_BossShadowAttack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_BossShadowAttack : public UGA_BossTargetBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector) override;

	void OnTimerConfirm();

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	UPROPERTY(EditDefaultsOnly)
	float ExecuteTime = 0.5f;

	float GetTargetDistance(AActor* ShadowTargetActor);

};
