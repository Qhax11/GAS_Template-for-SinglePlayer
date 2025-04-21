// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialAbilityInfo.generated.h"

UCLASS(Blueprintable)
class GAS_TEMPLATESP_API UW_TutorialAbilityInfo : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct(); 

	UPROPERTY(BlueprintReadOnly)
	FTutorialStepData TutorialStepData;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_AbilityName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_AbilityDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* AbilityVisual;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* B_ConfirmButton;

};
