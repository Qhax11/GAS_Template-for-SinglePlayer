// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AC_MeleeComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboMeleeAbilityEnded, const bool, bWasCancelled);

UENUM(BlueprintType)
enum class EComboType : uint8
{
	GroundCombo,
	AirCombo,
	ShadowCombo
};

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
	EComboType ComboType = EComboType::GroundCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboAbilityData> ComboAbilities;
};

UCLASS(BlueprintType)
class UComboChainsAsset : public UDataAsset
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
	int32 CurrentStepIndex = 0;

	UPROPERTY()
	TSubclassOf<UGA_ComboMeleeAttack> CurrentAbilityClass = nullptr;

	UPROPERTY()
	UGameplayAbility* CurrentAbilityInstance = nullptr;

	UPROPERTY()
	FGameplayAbilitySpecHandle CurrentAbilitySpecHandle;

	UPROPERTY()
	bool bIsActive = false;

	bool IsCurrentComboValid() const
	{
		return ComboChain.ComboAbilities.IsValidIndex(CurrentStepIndex);
	}

	bool IsChainFinished() const
	{
		return CurrentStepIndex >= ComboChain.ComboAbilities.Num();
	}

	const FComboAbilityData* GetCurrentCombo() const
	{
		return IsCurrentComboValid() ? &ComboChain.ComboAbilities[CurrentStepIndex] : nullptr;
	}

	void Advance()
	{
		++CurrentStepIndex;
	}

	void Reset()
	{
		CurrentStepIndex = 0;
		CurrentAbilityClass = nullptr;
		CurrentAbilityInstance = nullptr;
		CurrentAbilitySpecHandle = FGameplayAbilitySpecHandle();
		bIsActive = false;
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
	virtual UGA_ComboMeleeAttack* ActivateComboMelee(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag());

	UFUNCTION(BlueprintCallable)
	void StopCombo();

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

protected:
	virtual void BeginPlay() override;

	virtual void InitComboChainTracker(EComboType NewComboType);

	UFUNCTION()
	virtual void OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData);

	FComboChainSearchResult GetComboChainOfSelectedComboAbility(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass);

	void CancelComboAbilities();

	UPROPERTY(EditDefaultsOnly)
	UComboChainsAsset* ComboChainAsset;

	AGAS_CharacterBase* CharacterBase;
	UGAS_AbilitySystemComponent* CharacterBaseASC;
	FActiveComboChainTracker ActiveComboChainTracker;
	UGAS_GameplayAbilityBase* LastActivatedCombo;

private:
	void ContinueComboAfterCurrentStepEnded(bool bWasCancelled);
};
