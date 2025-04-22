// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/TriggerBox.h"
#include "GameplayTagContainer.h"
#include "A_TutorialTrigger.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AA_TutorialTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TutorialTag;

};
