// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroMeleeComboManager.generated.h"

UENUM(BlueprintType)
enum class EHeroComboType : uint8
{
	GroundCombo,
	AirCombo,
	ShadowCombo
};

UCLASS(BlueprintType)
class UHeroComboChainsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FComboChainData GroundCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FComboChainData AirCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FComboChainData ShadowCombo;
};

UCLASS()
class GAS_TEMPLATESP_API UAC_HeroMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void InitComboChain(EHeroComboType ComboType);

	void OnComboAbilityEnd(const FCustomAbilityEndedData& Data) override;

	void ChangeComboSet();

public:
	virtual UGA_ComboMeleeAttack* ActivateComboMelee(const FComboPreActivationData& Data = FComboPreActivationData()) override;

	UFUNCTION(BlueprintCallable)
	void StartShadowCombo(const FComboPreActivationData& Data);

	UPROPERTY(EditDefaultsOnly)
	UHeroComboChainsAsset* HeroComboAsset; 

private:
	UFUNCTION()
	void OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnPhaseActiveHitTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void FinishCombo();

	UPROPERTY()
	class AGAS_HeroBase* HeroBase;

	bool bComboChainConsumed = false;
};
