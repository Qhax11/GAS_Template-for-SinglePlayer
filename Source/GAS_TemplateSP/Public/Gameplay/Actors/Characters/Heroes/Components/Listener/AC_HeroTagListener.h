// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/GameplayTag/AC_TagListenerBase.h"
#include "AC_HeroTagListener.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_HeroTagListener : public UAC_TagListenerBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void BindTagDelegates() override;

	UPROPERTY()
	class AGAS_HeroBase* Hero;

	UPROPERTY()
	class UAC_HeroControl* HeroControlComp;

	UFUNCTION()
	void OnRunningTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnRunningTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnHeroTargetLockedTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnHeroTargetLockedTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnHeroFinisherTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnHeroFinisherTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
};
