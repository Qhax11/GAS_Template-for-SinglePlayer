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

	FEnemySpawnData() {}

	FEnemySpawnData(
		AGAS_CharacterBase* InCharacter,
		UAbilitySystemComponent* InASC)
		:
		Character(InCharacter),
		ASC(InASC)
	{
	}
};

UENUM(BlueprintType)
enum class EDeSpawnPhase : uint8
{
	DeathStarted UMETA(DisplayName = "Death Started"),
	DeathFinished UMETA(DisplayName = "Death Finished")
};

USTRUCT(BlueprintType)
struct FCharacterDeSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	EDeSpawnPhase DeSpawnPhase = EDeSpawnPhase::DeathStarted;

	UPROPERTY(BlueprintReadOnly)
	AGAS_CharacterBase* Character = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC = nullptr;

	// The character responsible for this character's death.
    // Typically the attacker who dealt the final blow.
	UPROPERTY(BlueprintReadOnly)
	const AGAS_CharacterBase* InstigatorCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* InstigatorASC = nullptr;

	FCharacterDeSpawnData() {}

	FCharacterDeSpawnData(EDeSpawnPhase InDeSpawnPhase, AGAS_CharacterBase* InCharacter, UAbilitySystemComponent* InASC,
		const AGAS_CharacterBase* InInstigatorCharacter, UAbilitySystemComponent* InInstigatorASC)
		: DeSpawnPhase(InDeSpawnPhase), Character(InCharacter), ASC(InASC), InstigatorCharacter(InInstigatorCharacter), InstigatorASC(InInstigatorASC)
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

// Brodcast from BeginPlay of PlayerController
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerSpawn, APlayerController*, PlayerController);

UCLASS()
class GAS_TEMPLATESP_API US_SpawnDelegates : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Enemy BeginPlay'de bunu çaðýracak (Artýk direkt Broadcast yapmayacaklar) */
	UFUNCTION(BlueprintCallable, Category = "Spawn Tracking")
	void RegisterEnemy(const FEnemySpawnData& SpawnData);

	/** Enemy öldüðünde listeden silmek için (Opsiyonel ama temizlik için iyi) */
	UFUNCTION(BlueprintCallable, Category = "Spawn Tracking")
	void UnregisterEnemy(const FCharacterDeSpawnData& DeSpawnData);

	/** Aktif olarak yaþayan düþmanlarýn listesi */
	UPROPERTY(BlueprintReadOnly, Category = "Spawn Tracking")
	TArray<FEnemySpawnData> AliveEnemies;

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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerControllerSpawn OnPlayerControllerSpawn;
};
