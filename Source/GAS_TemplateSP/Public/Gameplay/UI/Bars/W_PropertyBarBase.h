// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/TextBlock.h"
#include "Gameplay/Attributes/AS_Base.h"
#include "W_PropertyBarBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UW_PropertyBarBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void InitializePropertyBar(AActor* Owner);

	// Blueprint-callable C++ function; when called (from BP or C++), triggers the Blueprint-implemented fade-in animation event.
	UFUNCTION(BlueprintCallable)
	void PlayFadeInAnim();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOutAnim();

	// Blueprint-implementable event; meant to be implemented in Blueprint to define the actual fade-in animation behavior.
	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayFadeInAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayFadeOutAnim();

protected:
	virtual void SetDefaultValues(UAbilitySystemComponent* OwnerASC);

	virtual void BindAttributes(UAbilitySystemComponent* OwnerASC);
	
	void SetPercantage(float Value1, float Value2);

	void SetValuesToTexts(float CurrentValue, float MaxValue);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> T_CurrentValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> T_MaxValue;
};
