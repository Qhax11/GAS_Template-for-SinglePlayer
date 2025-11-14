// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GA_HeroJump.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UGA_HeroJump : public UGA_MontageAbility
{
	GENERATED_BODY()
	
protected:
	UGA_HeroJump();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void JumpLogic();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/** Jump yüksekliði (cm cinsinden) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings", meta = (ClampMin = "100", ClampMax = "2000"))
	float JumpHeight = 500.0f;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	class UAC_HeroControl* HeroControl;

	UPROPERTY()
	class UCharacterMovementComponent* HeroMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings")
	float GroundJumpForwardStrength = 1.0f;

	FTimerHandle JumpTimerHandle;
};
