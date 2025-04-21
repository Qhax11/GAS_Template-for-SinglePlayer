// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialQuest.generated.h"

USTRUCT(BlueprintType)
struct FTutorialQuestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "UI")
	FText QuestTitle;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText QuestInstruction;
};

UCLASS(Blueprintable)
class GAS_TEMPLATESP_API UW_TutorialQuest : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	void InitWithTutorialData(const FTutorialStepData& StepData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_QuestFinished();

protected:
	UPROPERTY(EditDefaultsOnly)
	FTutorialQuestData TutorialQuestData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_QuestInstruction;
	
};
