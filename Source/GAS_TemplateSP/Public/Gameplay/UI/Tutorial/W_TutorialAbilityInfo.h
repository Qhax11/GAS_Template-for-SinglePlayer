// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialAbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FTutorialAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "UI")
	FText AbilityName;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText AbilityDescription;
};

UCLASS(Blueprintable)
class GAS_TEMPLATESP_API UW_TutorialAbilityInfo : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	void InitWithTutorialData(const FTutorialStepData& StepData);
	
	UPROPERTY(BlueprintReadOnly)
	FTutorialStepData TutorialStepData;

protected:
	UPROPERTY(EditDefaultsOnly)
	FTutorialAbilityData TutorialAbilityData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_AbilityName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_AbilityDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* AbilityVisual;
};
