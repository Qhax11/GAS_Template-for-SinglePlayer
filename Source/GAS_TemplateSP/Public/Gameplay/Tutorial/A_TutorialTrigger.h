// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/TriggerBox.h"
#include "A_TutorialTrigger.generated.h"

struct FGameplayTag;

UCLASS()
class GAS_TEMPLATESP_API AA_TutorialTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TutorialTag;

};
