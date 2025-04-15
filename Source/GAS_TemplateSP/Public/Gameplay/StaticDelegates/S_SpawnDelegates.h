// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "S_SpawnDelegates.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* CharacterBase = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;
};

// Spawn refers to the first initialization, 
// ReSpawn refers to subsequent initializations after death
// DeSpawn means the character's death.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSpawn, const FCharacterSpawnData&, HeroSpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroReSpawn, const FCharacterSpawnData&, HeroSpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroDeSpawn, const FCharacterSpawnData&, HeroSpawnData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawn, const FCharacterSpawnData&, EnemySpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyReSpawn, const FCharacterSpawnData&, EnemySpawnData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeSpawn, const FCharacterSpawnData&, EnemySpawnData);


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
