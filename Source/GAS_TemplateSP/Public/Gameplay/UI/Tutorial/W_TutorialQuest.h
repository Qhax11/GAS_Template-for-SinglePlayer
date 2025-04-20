// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialQuest.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UW_TutorialQuest : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	void InitWithTutorialData(const FTutorialStepData& StepData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_QuestFinished();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_QuestInstruction;
	
};
