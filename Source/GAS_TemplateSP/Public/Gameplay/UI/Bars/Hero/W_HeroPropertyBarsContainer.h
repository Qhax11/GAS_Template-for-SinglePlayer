// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Blueprint/UserWidget.h"
#include "W_HeroPropertyBarsContainer.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_HeroPropertyBarsContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitalizePropertyBarsContainer(AActor* Owner);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UW_HealthBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UW_PostureBar> PostureBar;
};
