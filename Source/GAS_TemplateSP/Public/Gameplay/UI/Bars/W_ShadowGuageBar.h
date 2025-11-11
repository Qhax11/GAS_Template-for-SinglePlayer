// Qhax's GAS Template for SinglePlayer

#pragma once
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/TextBlock.h"
#include "AttributeSet.h"
#include "W_ShadowGuageBar.generated.h"

/**
 * Generic Property Bar that can bind to any attribute pair (Current/Max)
 */
UCLASS()
class GAS_TEMPLATESP_API UW_ShadowGuageBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void InitializePropertyBar(AActor* Owner);

	UFUNCTION(BlueprintCallable)
	void PlayFadeInAnim();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOutAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayFadeInAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayFadeOutAnim();

	// Blueprint event called when attribute changes
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAttributeChanged(float CurrentValue, float MaxValue, float OldValue);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetPercentage(float Value1, float Value2);
	void SetValuesToTexts(float CurrentValue, float MaxValue);

	// Generic attribute binding
	void BindToAttribute(UAbilitySystemComponent* OwnerASC);
	void OnCurrentAttributeChanged(const FOnAttributeChangeData& Data);
	void OnMaxAttributeChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> T_CurrentValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> T_MaxValue;

	// Blueprint'ten set edilecek attribute'lar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttribute CurrentAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttribute MaxAttribute;

private:
	// Delegate handles for cleanup
	FDelegateHandle CurrentAttributeHandle;
	FDelegateHandle MaxAttributeHandle;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	float CachedCurrentValue = 0.f;
	float CachedMaxValue = 1.f;
};