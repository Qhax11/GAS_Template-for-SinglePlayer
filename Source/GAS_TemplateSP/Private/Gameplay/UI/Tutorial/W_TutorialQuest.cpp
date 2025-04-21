// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Tutorial/W_TutorialQuest.h"
#include "Components/TextBlock.h"

void UW_TutorialQuest::NativeConstruct()
{
	if (T_QuestInstruction)
	{
		T_QuestInstruction->SetText(TutorialQuestData.QuestInstruction);
	}
}

void UW_TutorialQuest::InitWithTutorialData(const FTutorialStepData& StepData)
{
	/*
	FTutorialQuestData QuestData = StepData.QuestData;

	if (T_QuestInstruction)
	{
		T_QuestInstruction->SetText(QuestData.QuestInstruction);
	}
	*/
}

