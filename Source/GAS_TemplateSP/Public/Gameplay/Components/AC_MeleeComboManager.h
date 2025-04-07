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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MontageSection;
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
	virtual void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None);

	UFUNCTION(BlueprintCallable)
	void StopCombo();

protected:
	virtual void BeginPlay() override;

	void CancelComboAbilities();

	AGAS_CharacterBase* CharacterBase;
	UAbilitySystemComponent* CharacterBaseASC;

	UFUNCTION()
	virtual void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	FComboChainSearchResult GetComboChainOfSelectedComboAbility(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass);

	UFUNCTION()
	void OnCanActivateNextAttack();

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

	UPROPERTY(EditDefaultsOnly)
	UComboChainAsset* ComboChainAsset;

	FActiveComboChainTracker ActiveComboChainTracker;
};
