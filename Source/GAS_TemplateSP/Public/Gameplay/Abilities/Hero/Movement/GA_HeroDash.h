// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DashBase.h"
#include "GA_HeroDash.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDash : public UGA_DashBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual FVector CalculateDestination() override;

	FVector GetDashDirectionFromHeroLastMovement();

	FVector GetDirectionFromLastMovementInput(const FVector2D& LastMovementInput);

	// Abilities that have the specified tags in this container will be canceled before executing this ability.
    // This is useful when ensuring that conflicting abilities (e.g., those playing root motion montages) do not interfere.
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CancelAbilityTags;
	
	class AGAS_HeroBase* HeroBase;
};
