// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroEnemyAttackListener.generated.h"

class UGameplayAbility;
struct FEnemySpawnData;
struct FGameplayTagContainer;
struct FTimerHandle;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_HeroEnemyAttackListener : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_HeroEnemyAttackListener();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnemySpawn(const FEnemySpawnData& EnemySpawnData);

	void OnEnemyAbilityActivated(UGameplayAbility* Ability);

	void OnEnemyAbilityEnded(UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags);

	UPROPERTY(EditDefaultsOnly)
	float PerfectOffsetStart = 0.10f;

	UPROPERTY(EditDefaultsOnly)
	float PerfectOffsetEnd = 0.05f;

	TMap<UGameplayAbility*, FTimerHandle> ActiveAbilityTimers_Start;
	TMap<UGameplayAbility*, FTimerHandle> ActiveAbilityTimers_End;

	int32 ActivePerfectWindows = 0;

	void CleanupAbilityTimers(UGameplayAbility* Ability);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

};