// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AC_MeleeComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboMeleeAbilityEnded, const bool, bWasCancelled);

USTRUCT(BlueprintType)
struct FComboAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGA_ComboMeleeAttack> ComboAbilityClass;
};

USTRUCT(BlueprintType)
struct FComboChainData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ComboChainName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboAbilityData> ComboAbilities;
};

UCLASS(BlueprintType)
class UComboChainAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboChainData> ComboChains;
};

USTRUCT()
struct FActiveComboChainTracker
{
	GENERATED_BODY()

	UPROPERTY()
	FComboChainData ComboChain = FComboChainData();

	UPROPERTY()
	int32 CurrentIndex = 0;

	UPROPERTY()
	TSubclassOf<UGA_ComboMeleeAttack> CurrentAbilityClass = nullptr;

	UPROPERTY()
	UGameplayAbility* CurrentAbilityInstance = nullptr;

	UPROPERTY()
	FGameplayAbilitySpecHandle CurrentAbilitySpecHandle;

	UPROPERTY()
	bool bNextAttackAllowed = true;

	bool IsCurrentComboValid() const
	{
		return ComboChain.ComboAbilities.IsValidIndex(CurrentIndex);
	}

	bool IsChainFinished() const
	{
		return CurrentIndex >= ComboChain.ComboAbilities.Num();
	}

	const FComboAbilityData* GetCurrentCombo() const
	{
		return IsCurrentComboValid() ? &ComboChain.ComboAbilities[CurrentIndex] : nullptr;
	}

	void Advance()
	{
		++CurrentIndex;
	}

	void Reset()
	{
		CurrentIndex = 0;
		CurrentAbilityClass = nullptr;
		CurrentAbilityInstance = nullptr;
		CurrentAbilitySpecHandle = FGameplayAbilitySpecHandle();
		bNextAttackAllowed = true;
	}
};

USTRUCT(BlueprintType)
struct FComboChainSearchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FComboChainData ComboChain;

	UPROPERTY(BlueprintReadOnly)
	int32 FindedComboIndex = INDEX_NONE;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_MeleeComboManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_MeleeComboManager();

	UFUNCTION(BlueprintCallable)
	virtual UGA_ComboMeleeAttack* ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag());

	UFUNCTION(BlueprintCallable)
	void StopCombo();

protected:
	virtual void BeginPlay() override;

	virtual void InitComboChainTracker();

	UFUNCTION()
	virtual void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	FComboChainSearchResult GetComboChainOfSelectedComboAbility(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass);

	void CancelComboAbilities();

	UPROPERTY(EditDefaultsOnly)
	UComboChainAsset* ComboChainAsset;

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

	// Can be set from UI in the future to allow different combo styles.
	int32 SelectedComboIndex = 0;
	AGAS_CharacterBase* CharacterBase;
	UAbilitySystemComponent* CharacterBaseASC;
	FActiveComboChainTracker ActiveComboChainTracker;
};
