// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_SendTag.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAN_SendTag : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Categories = "Gameplay.Event.AnimNotify"))
	FGameplayTag NotifyTag;

};
