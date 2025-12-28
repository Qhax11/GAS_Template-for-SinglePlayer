// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

UENUM(BlueprintType)
enum class EEnemyComboChainResult : uint8
{
	Completed,          // Tüm combo baþarýyla bitti
	HitTaken,          // damage aldý
	ParryTriggered,    // parry seçildi
	DodgeTriggered,    // dodge / evade
	Cancelled,          // Ability cancel oldu (generic)
	OutOfRange,         // Mesafe yüzünden devam edemedi
};

USTRUCT(BlueprintType)
struct FEnemyComboChainEndData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EEnemyComboChainResult Result = EEnemyComboChainResult::Cancelled;

	UPROPERTY(BlueprintReadOnly)
	int32 EndedAtComboIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	UGameplayAbility* LastComboAbility = nullptr;
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

UCLASS(BlueprintType)
class UEnemyComboChainAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FComboChainData ComboChain;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyComboChainEnded, const FEnemyComboChainEndData&);

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartComboChain(UEnemyComboChainAsset* ComboChain, FName MontageSection = NAME_None);

	FOnEnemyComboChainEnded OnEnemyComboChainEnded;

protected:
	virtual void BeginPlay() override;

	virtual UGA_ComboMeleeAttack* ActivateComboMelee(const FComboPreActivationData& Data = FComboPreActivationData()) override;

	void OnComboAbilityEnd(const FCustomAbilityEndedData& Data) override;
	
private:
	void FinishComboChain(EEnemyComboChainResult Result);

	void BroadcastComboChainEnd(UGameplayAbility* LastComboAbility, EEnemyComboChainResult Result);

	UPROPERTY()
	class AAIControllerBase* AIController;

	UPROPERTY()
	class UAC_TagDelegates* EnemyTagDelegatesComp;

	bool bOnTakeDamageState;
};
