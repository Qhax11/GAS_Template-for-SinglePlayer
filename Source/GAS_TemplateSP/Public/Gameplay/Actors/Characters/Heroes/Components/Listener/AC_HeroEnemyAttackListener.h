// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroEnemyAttackListener.generated.h"

struct FEnemySpawnData;
struct FGameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroEnemyAttackListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroEnemyAttackListener();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnemySpawn(const FEnemySpawnData& EnemySpawnData);

	void OnEnemyAbilityActivated(class UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags);

	float PerfectOffsetStart = 0.10f;

	float PerfectOffsetEnd = 0.05f;


};
