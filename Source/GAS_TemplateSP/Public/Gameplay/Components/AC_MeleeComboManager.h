// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Abilities/DataTypes/Combo/ComboPreActivationData.h"
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

	void StartChain(const FComboChainData& InComboChain)
	{
		ComboChain = InComboChain;
		CurrentStepIndex = 0;
	}

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

	virtual void OnComboAbilityActivated(UGA_ComboMeleeAttack* Instance);

	UFUNCTION(BlueprintCallable)
	void StopCombo();

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateComboMelee(const UComboPreActivationData* Data = nullptr);

	UFUNCTION()
	virtual void OnComboAbilityEnd(const FCustomAbilityEndedData& Data);

	void CancelComboAbilities();

	void ClearComboChain();

	UPROPERTY()
	AGAS_CharacterBase* CharacterBase;

	UPROPERTY()
	UGAS_AbilitySystemComponent* CharacterBaseASC;

	FActiveComboChainTracker ActiveComboChainTracker;
};
