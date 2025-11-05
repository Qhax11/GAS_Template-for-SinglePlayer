// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroJump.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UGA_HeroJump : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	// Jump curve ticker
	UFUNCTION()
	void TickJumpCurve();

private:
	// --- Configuration ---

	/** Jump yüksekliði (cm cinsinden) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings", meta = (ClampMin = "100", ClampMax = "2000"))
	float JumpHeight = 500.0f;

	/** Jump süresi (saniye) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float JumpDuration = 0.8f;

	/** Jump trajectory curve (0-1 range, Y ekseni velocity multiplier) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings")
	UCurveFloat* JumpCurve;

	/** Gravity scale jump sýrasýnda (daha fazla kontrol için) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings", meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float JumpGravityScale = 1.0f;

	/** Jump baþlangýcýnda uygulanan impulse (ekstra pop hissi için) */
	UPROPERTY(EditDefaultsOnly, Category = "Jump Settings")
	float InitialImpulse = 100.0f;

	/** InAir tag - Bu tag varsa jump aktif edilemez */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag InAirTag;

	// --- Runtime Variables ---

	FTimerHandle JumpTimerHandle;
	float JumpElapsedTime;
	float OriginalGravityScale;
	class UCharacterMovementComponent* CharacterMovement;
	class ACharacter* OwnerCharacter;

	// Helper functions
	void ApplyJumpVelocity(float DeltaTime);
	void RestoreGravity();
	float CalculateInitialVelocity() const;


};
