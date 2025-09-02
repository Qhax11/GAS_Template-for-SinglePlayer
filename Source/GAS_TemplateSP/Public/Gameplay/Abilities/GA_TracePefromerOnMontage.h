// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TracePefromerOnMontage")
	float TraceTickValue = 0.01f;

	FTimerHandle TimerHandle_TraceTick;

	UPROPERTY()
	class AGAS_CharacterBase* CharacterBase;
};
