// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_SpawnDelegates.generated.h"

class UAbilitySystemComponent;
class AGAS_CharacterBase;
class UST_Base;

USTRUCT(BlueprintType)
struct FHeroSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* Character = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;

	FHeroSpawnData() {}

	FHeroSpawnData(AGAS_CharacterBase* InCharacter, UAbilitySystemComponent* InASC)
		: Character(InCharacter), ASC(InASC)
	{}
};

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* Character = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UST_Base* StateTree = nullptr;

	FEnemySpawnData() {}

	FEnemySpawnData(AGAS_CharacterBase* InCharacter, UAbilitySystemComponent* InASC, UST_Base* InStateTree)
		: Character(InCharacter), ASC(InASC), StateTree(InStateTree)
	{}
};

USTRUCT(BlueprintType)
struct FCharacterDeSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* Character = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;

	// The character responsible for this character's death.
    // Typically the attacker who dealt the final blow.
	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* InstigatorCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* InstigatorASC = nullptr;

	FCharacterDeSpawnData() {}

	FCharacterDeSpawnData(AGAS_CharacterBase* InCharacter, UAbilitySystemComponent* InASC,
		AGAS_CharacterBase* InInstigatorCharacter, UAbilitySystemComponent* InInstigatorASC)
		: Character(InCharacter), ASC(InASC), InstigatorCharacter(InInstigatorCharacter), InstigatorASC(InInstigatorASC)
	{}
};

// Spawn refers to the first initialization, 
// ReSpawn refers to subsequent initializations after death
// DeSpawn means the character's death.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSpawn, const FHeroSpawnData&, HeroSpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroReSpawn, const FHeroSpawnData&, HeroReSpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroDeSpawn, const FCharacterDeSpawnData&, HeroDeSpawnData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawn, const FEnemySpawnData&, EnemySpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyReSpawn, const FEnemySpawnData&, EnemyReSpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeSpawn, const FCharacterDeSpawnData&, EnemyDeSpawnData);


UCLASS()
class GAS_TEMPLATESP_API US_SpawnDelegates : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable)
	FOnHeroSpawn OnHeroSpawn;

	UPROPERTY(BlueprintAssignable)
	FOnHeroReSpawn OnHeroReSpawn;

	UPROPERTY(BlueprintAssignable)
	FOnHeroDeSpawn OnHeroDeSpawn;

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawn OnEnemySpawn;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyReSpawn OnEnemyReSpawn;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeSpawn OnEnemyDeSpawn;
};
