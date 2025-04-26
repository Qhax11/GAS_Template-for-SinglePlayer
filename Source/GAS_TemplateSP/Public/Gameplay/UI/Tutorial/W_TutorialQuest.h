// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/UI/Tutorial/W_TutorialBase.h"
#include "W_TutorialQuest.generated.h"


UCLASS(Blueprintable)
class GAS_TEMPLATESP_API UW_TutorialQuest : public UW_TutorialBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_QuestInstruction;
	
};
