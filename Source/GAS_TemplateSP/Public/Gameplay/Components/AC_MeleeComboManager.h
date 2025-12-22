// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
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
	TArray<FComboAbilityData> ComboAbilities;
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
	UGameplayAbility* CurrentAbilityInstance = nullptr;

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
		CurrentAbilityInstance = nullptr;
		bIsActive = false;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_MeleeComboManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_MeleeComboManager();

	UFUNCTION(BlueprintCallable)
	void StopCombo();

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual UGA_ComboMeleeAttack* ActivateComboMelee(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag());

	UFUNCTION()
	virtual void OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData);

	void CancelComboAbilities();

	UPROPERTY()
	AGAS_CharacterBase* CharacterBase;

	UPROPERTY()
	UGAS_AbilitySystemComponent* CharacterBaseASC;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastActivatedCombo;

	FActiveComboChainTracker ActiveComboChainTracker;
};
