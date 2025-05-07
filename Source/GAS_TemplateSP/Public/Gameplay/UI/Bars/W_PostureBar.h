// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Bars/W_PropertyBarBase.h"
#include "W_PostureBar.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_PostureBar : public UW_PropertyBarBase
{
	GENERATED_BODY()
	
protected:
	virtual void SetDefaultValues(UAbilitySystemComponent* OwnerASC) override;

	virtual void BindAttributes(UAbilitySystemComponent* OwnerASC) override;

	UFUNCTION()
	void PostureChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	void MaxPostureChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPostureChanged(const FAttributeChangeCallbackData& Data);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> BackgroundProgressBar;
};
