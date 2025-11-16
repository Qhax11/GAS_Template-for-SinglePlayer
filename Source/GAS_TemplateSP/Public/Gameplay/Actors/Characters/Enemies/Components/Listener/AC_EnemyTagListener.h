// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "AC_EnemyTagListener.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyTagListener : public UAC_TagListenerBase
{
	GENERATED_BODY()
	
protected:
	virtual void BindTagDelegates() override;

	UFUNCTION()
	void OnPatrollingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnPatrollingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnStrafingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnStrafingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
};
