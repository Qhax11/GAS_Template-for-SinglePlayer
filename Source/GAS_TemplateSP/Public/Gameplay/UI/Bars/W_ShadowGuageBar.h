// Qhax's GAS Template for SinglePlayer

#pragma once
#include "Gameplay/UI/Bars/W_PropertyBarBase.h"
#include "W_ShadowGuageBar.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_ShadowGuageBar : public UW_PropertyBarBase
{
	GENERATED_BODY()

protected:
	virtual void SetDefaultValues(UAbilitySystemComponent* OwnerASC) override;

	virtual void BindAttributes(UAbilitySystemComponent* OwnerASC) override;

	UFUNCTION()
	void ShadowGuageChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	void MaxShadowGuageChanged(const FAttributeChangeCallbackData& Data);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> BackgroundProgressBar;
};