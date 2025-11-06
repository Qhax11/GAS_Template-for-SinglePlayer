// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "GA_MeleeFinisher.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeFinisher : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

public:
	UGA_MeleeFinisher();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetFinisherTypeTagFromAbilityTags() const;
};
