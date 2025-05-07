// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/WidgetComponent.h"
#include "WC_PropertyBarsContainer.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UWC_PropertyBarsContainer : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnAbilitySetGiven(const AActor* OwnerActor);

	TObjectPtr<class UW_PropertyBarsContainer> PropertyBarsContainer;
};
