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

	// Returns true if the attack request was successful.
	UFUNCTION(BlueprintCallable)
	bool RequestToBeAttackIntender(UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	void OnNewAttackIntenderAdded(UAbilitySystemComponent* NewIntender);

	UFUNCTION(BlueprintCallable)
	bool ReleaseAttackIntender(UAbilitySystemComponent* ASC);

protected:
	void AddAttackIntender(UAbilitySystemComponent* ASC);

	bool RemoveAttackIntender(UAbilitySystemComponent* ASC);

	UAbilitySystemComponent* GetFurthestAttackIntender(UAbilitySystemComponent* IgnoreASC) const;

	// Loops through all enemies that are not currently attack intenders.
    // Compares distance to the hero, and returns the closest valid one.
	UAbilitySystemComponent* GetClosestNonAttackIntender(UAbilitySystemComponent* IgnoreASC) const;

	UFUNCTION()
	void OnHeroSpawn(AGAS_CharacterBase* CharacterBase);
	AGAS_CharacterBase* Hero;

	UFUNCTION()
	void OnEnemySpawn(AGAS_CharacterBase* CharacterBase);

	UFUNCTION()
	void OnEnemyDeSpawn(AGAS_CharacterBase* CharacterBase);

#if WITH_EDITOR
	void DebugPrintState();
#endif // WITH_EDITOR

protected:
	TArray<UAbilitySystemComponent*> AllEnemies;
	TArray<UAbilitySystemComponent*> AttackIntenders;
	TArray<UAbilitySystemComponent*> NonAttackIntenders;

private:
	int32 MaxEnemyAttackingCount = 0;
	bool bDebug;

};
