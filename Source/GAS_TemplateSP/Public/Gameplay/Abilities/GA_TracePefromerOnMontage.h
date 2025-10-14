// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include "GA_TracePefromerOnMontage.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_TracePefromerOnMontage : public UGA_MontageAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void TraceTick();

	bool TraceForHostileUnits(TArray<FHitResult>& OutHitResults);

	virtual void OnTraceHitResults(const TArray<FHitResult>& HitResults);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TracePefromerOnMontage")
	float TraceTickValue = 0.01f;

	FTimerHandle TimerHandle_TraceTick;

	UPROPERTY()
	AGAS_CharacterBase* CharacterBase;

	UPROPERTY()
	AWeaponBase* CharacterWeapon;
};
