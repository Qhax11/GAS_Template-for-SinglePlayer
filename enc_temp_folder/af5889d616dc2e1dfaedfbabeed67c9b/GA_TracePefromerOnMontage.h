// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include "GA_TracePefromerOnMontage.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_TracePefromerOnMontage : public UGA_MontageAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void GetTracePoints(FVector& OutStart, FVector& OutEnd, FRotator& OutRot);

	virtual void OnTraceHitResults(const TArray<FHitResult>& HitResults);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY()
	TObjectPtr<class AWeaponBase> CharacterWeapon;
	// Interpolated trace settings
	UPROPERTY(EditDefaultsOnly, Category = "Trace", meta = (ClampMin = "20.0", ClampMax = "50.0"))
	float MaxStepSize = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float MinDistanceThreshold = 1.0f; 

	// Previous frame positions
	FVector PrevTraceStart;
	FVector PrevTraceEnd;
	bool bIsFirstTraceTick = true;

	// Hit tracking
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActorsThisSwing;

private:
	// Interpolated trace logic
	void PerformInterpolatedTrace();

	// Trace helper
	bool TraceForHostileUnits(const FVector& Start, const FVector& End, TArray<FHitResult>& OutHitResults);
};
