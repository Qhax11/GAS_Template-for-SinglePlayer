// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Hero/GA_HeroTargetBase.h"
#include "GA_HeroShadowFinisher.generated.h"

// The UGA_HeroShadowFinisher class is responsible for triggering the actual finisher class  

UCLASS()
class GAS_TEMPLATESP_API UGA_HeroShadowFinisher : public UGA_HeroTargetBase
{
	GENERATED_BODY()

protected:
	UGA_HeroShadowFinisher();

	/** Override this to control whether the ability can activate */
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector);

	virtual void OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGA_MeleeAttackBase> FinisherAbilityClass;

	virtual void CancelAbilityFromInput() override;
};
