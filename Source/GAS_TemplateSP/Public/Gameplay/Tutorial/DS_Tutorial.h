// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TriggerBox.h"
#include "DS_Tutorial.generated.h"


UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Tutorial"))
class GAS_TEMPLATESP_API UDS_Tutorial : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Tutorial")
	TMap<TSoftObjectPtr<ATriggerBox>, TSoftClassPtr<UUserWidget>> TutorialWidgetsByTrigger;

};
