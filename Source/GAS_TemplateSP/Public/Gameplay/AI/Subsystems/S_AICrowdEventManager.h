// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_AICrowdEventManager.generated.h"

class AGAS_CharacterBase;
class AAIController;
struct FGameplayTag;
class UAbilitySystemComponent;

USTRUCT()
struct FEnemyData
{
	GENERATED_BODY()

public:
	UAbilitySystemComponent* EnemyASC = nullptr;
	AAIController* EnemyController = nullptr;

	FEnemyData() = default;

	FEnemyData(UAbilitySystemComponent* InEnemyASC, AAIController* InEnemyController)
		: EnemyASC(InEnemyASC),
		EnemyController(InEnemyController)
	{}

};

UCLASS()
class GAS_TEMPLATESP_API US_AICrowdEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnEnemySpawn(AGAS_CharacterBase* CharacterBase);

	UFUNCTION()
	void OnAttackStateTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnAttackStateTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

protected:
	TArray<FEnemyData> EnemyData;

	void SetValueToBlackboards(bool Value);

	int32 MovingToAttackCount;

private:
	int32 MaxMovingToAttackCount = 0;
	bool bDebug;
};
