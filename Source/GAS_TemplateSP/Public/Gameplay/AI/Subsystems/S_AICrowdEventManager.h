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
	void OnHeroSpawn(AGAS_CharacterBase* CharacterBase);
	AGAS_CharacterBase* Hero;

	UFUNCTION()
	void OnEnemySpawn(AGAS_CharacterBase* CharacterBase);

public:
	// AI listening states
	UFUNCTION()
	void OnAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnAttackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnMoveToAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnMoveToAttackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnSoCloseToHeroTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnSoCloseToHeroTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

protected:
	TArray<FEnemyData> AllEnemies;

	TArray<FEnemyData> MoveToAttackEnemies;

	void SetValueToBlackboards(bool Value);

private:
	int32 MaxEnemyAttackingCount = 0;
	int32 EnemyAttackingCount = 0;
	bool bDebug;

};
