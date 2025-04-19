// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Blueprint/UserWidget.h"
#include "Gameplay/Tutorial/DS_Tutorial.h"
#include "W_TutorialBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_TutorialBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitWithTutorialData(const FTutorialStepData& Data);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_AbilityName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_AbilityDescription;

	UPROPERTY(meta = (BindWidget))
	class UImage* AbilityVisual;
};
