// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRouter.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRouter : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Router")
	TArray<TSubclassOf<UGAS_GameplayAbilityBase>> AbilitiesToTry;
};
