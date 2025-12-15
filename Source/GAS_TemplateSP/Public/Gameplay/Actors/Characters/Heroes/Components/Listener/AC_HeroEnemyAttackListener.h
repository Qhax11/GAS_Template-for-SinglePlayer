// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroBase.h"
#include "AC_HeroEnemyAttackListener.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UGA_MeleeAttackBase;
struct FEnemySpawnData;
struct FGameplayTagContainer;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_HeroEnemyAttackListener : public UAC_HeroBase
{
	GENERATED_BODY()

public:
	UAC_HeroEnemyAttackListener();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnemySpawn(const FEnemySpawnData& EnemySpawnData);

	void OnEnemyAbilityActivated(UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability);

	void ApplyPerfectWindowTagEffect();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditDefaultsOnly, Category = "Attack|PerfectWindow")
	TSubclassOf<UGameplayEffect> PerfectWindowTagEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "PerfectWindow")
	float PerfectWindowEarlyOffset = 0.10f;

	UPROPERTY()
	TArray<FTimerHandle> PendingPerfectWindowTimers;
};