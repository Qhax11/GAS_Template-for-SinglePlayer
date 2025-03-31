// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TargetBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "GA_BossTargetBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_BossTargetBase : public UGA_TargetBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	AAIControllerBase* BossController;

	AGAS_EnemyBase* BossCharacter;
};
