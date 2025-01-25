// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_AICrowdEventManager.generated.h"

class AGAS_CharacterBase;
class AAIController;
struct FGameplayTag;
class UAbilitySystemComponent;

UCLASS()
class GAS_TEMPLATESP_API US_AICrowdEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnEnemySpawn(AGAS_CharacterBase* CharacterBase);

	UFUNCTION()
	void OnMovingToAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnMovingToAttackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void CheckUpdatedMovingToAttackCount(const UAbilitySystemComponent* UpdaterASC);

protected:
	TMap<UAbilitySystemComponent*, AAIController*> EnemyData;

	void SetValueToBlackboards(const UAbilitySystemComponent* UpdaterASC, bool Value);

	int32 MovingToAttackCount;

private:
	int32 MaxMovingToAttackCount = 0;
	bool bDebug;
};
