// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

UENUM(BlueprintType)
enum class EEnemyComboChainResult : uint8
{
	Completed,          // Tüm combo baþarýyla bitti
	Cancelled,          // Ability cancel oldu (generic)
	InterruptedByHit,   // TakeDamage yüzünden kesildi
	OutOfRange,         // Mesafe yüzünden devam edemedi
	InvalidStep         // Data / ability invalid
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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyComboChainEnded, const FEnemyComboChainEndData&);

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass, FName MontageSection = NAME_None);

	UFUNCTION(BlueprintCallable)
	float GetMaxRangeOfCurrentAttack();

	FOnEnemyComboChainEnded OnEnemyComboChainEnded;

protected:
	virtual void BeginPlay() override;

	virtual UGA_ComboMeleeAttack* ActivateComboMelee(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag()) override;

	UFUNCTION()
	void OnEnemyCanActivateNextAttack();

	UFUNCTION()
	void OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void OnComboAbilityEnd(const FCustomAbilityEndedData& EndedData) override;
	
private:
	void BroadcastComboChainEnd(EEnemyComboChainResult Result);

	class AAIControllerBase* AIController;
	class UAC_TagDelegates* EnemyTagDelegatesComp;
	bool bOnTakeDamageState;
};
