// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "S_AICrowdEventManager.generated.h"

class AAIController;
class UST_Base;
struct FCharacterSpawnData;
struct FGameplayTag;

USTRUCT()
struct FEnemyData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UAbilitySystemComponent* ASC = nullptr;

	UPROPERTY()
	UST_Base* StateTree = nullptr;

	FEnemyData() {}

	FEnemyData(UAbilitySystemComponent* InASC, UST_Base* InStateTree)
		: ASC(InASC), StateTree(InStateTree)
	{}

	// Equals operator for array operations
	bool operator==(const FEnemyData& Other) const
	{
		return ASC == Other.ASC;
	}

	// Check attack intent via tag
	bool IsAttackIntender() const
	{
		return ASC && ASC->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender);
	}

	// Set or remove intent tag on ASC
	void SetAttackIntender(bool bWantsToAttack)
	{
		if (!ASC)
		{
			return;
		}

		if (bWantsToAttack)
		{
			if (!ASC->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender))
			{
				ASC->AddLooseGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender);
			}
		}
		else
		{
			if (ASC->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender))
			{
				ASC->RemoveLooseGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender);
			}
		}
	}

};

UCLASS()
class GAS_TEMPLATESP_API US_AICrowdEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// ===============================================================
    //                                Initialization
    // ===============================================================
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnHeroSpawn(const FHeroSpawnData& HeroSpawnData);

	UFUNCTION()
	void OnEnemySpawn(const FEnemySpawnData& EnemySpawnData);

	UFUNCTION()
	void OnEnemyDeSpawn(const FCharacterDeSpawnData& CharacterSpawnData);

	// ===============================================================
    //                           Public Control Interface
    // ===============================================================
public:
	/**
    * Attempts to register this ASC as an active attack intender.
    * Typically called by AI via State Tree when entering attack mode.
    * Will succeed only if the current number of intenders is below the allowed maximum.
    *
    * @return True if the ASC was accepted as an attack intender.
    */
	UFUNCTION(BlueprintCallable)
	bool RequestToBeAttackIntender(UAbilitySystemComponent* ASC);

	/**
    * Forces the given ASC into the intender list.
    * If the max attacker limit has been reached, the furthest attacker will be removed to make room.
    * Typically used to give priority to closer or more urgent enemies.
    */
	UFUNCTION(BlueprintCallable)
	void OnNewAttackIntenderAdded(UAbilitySystemComponent* NewIntender);

	/**
    * Removes the ASC from the attack intender list if it was in it.
    * Can be called when an AI finishes or aborts its attack, or is about to despawn.
    *
    * @return True if the ASC was found and removed.
    */
	UFUNCTION(BlueprintCallable)
	bool ReleaseAttackIntender(UAbilitySystemComponent* ASC);

	// ===============================================================
    //                                Internal Logic
    // ===============================================================
protected:
	bool AddAttackIntender(UAbilitySystemComponent* ASC);

	bool RemoveAttackIntender(UAbilitySystemComponent* ASC);

	/**
    * Safely removes an enemy from the internal Enemies array by matching ASC pointer.
    * This function avoids modifying the array using a direct reference to an element inside it,
    * which would cause undefined behavior and crash due to memory reallocation during TArray::Remove().
    *
    * Instead, this version uses IndexOfByPredicate() + RemoveAt() which is safe and reliable.
    *
    * @param ASC The ability system component of the enemy to remove.
    * @return True if the enemy was found and removed successfully.
    */
	bool SafeRemoveEnemyByASC(UAbilitySystemComponent* ASC);

	// ===============================================================
    //                                Query Helpers
    // ===============================================================
protected:
	FEnemyData* FindEnemyDataByASC(UAbilitySystemComponent* ASC);

	TArray<UAbilitySystemComponent*> GetAttackIntenders() const;

	TArray<UAbilitySystemComponent*> GetNonAttackIntenders() const;

	UAbilitySystemComponent* GetFurthestAttackIntender(UAbilitySystemComponent* IgnoreASC) const;

	UAbilitySystemComponent* GetClosestNonAttackIntender(UAbilitySystemComponent* IgnoreASC) const;

	// ===============================================================
    //                                Debug
    // ===============================================================
#if WITH_EDITOR
	void DebugPrintState();
#endif // WITH_EDITOR

private:
	/** All currently active enemy AI units tracked by the system. */
	TArray<FEnemyData> Enemies;

	/** The current player-controlled hero the AI references for targeting/distance logic. */
	AActor* HeroActor;

	int32 MaxEnemyAttackingCount = 0;
	bool bDebug;

};
