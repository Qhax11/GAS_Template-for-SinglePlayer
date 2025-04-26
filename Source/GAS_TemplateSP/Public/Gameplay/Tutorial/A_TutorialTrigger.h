// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/TriggerBox.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "A_TutorialTrigger.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AA_TutorialTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay.Tutorial"))
	FGameplayTag TutorialTag;

	/*
    * FAbilityData struct could not be used directly inside DeveloperSettings because StateTree serialization does not support nested UObject properties properly.
    * Using FAbilityData in a config-based asset caused build-time and runtime issues due to invalid UObject references during serialization.
    *
    * Therefore, we moved the usage of FAbilityData to an Actor/Component (Tutorial Trigger) instead of Developer Settings,
    * where it is fully supported because the asset references are resolved safely at runtime.
    *
    * This allows using FAbilityData normally without causing StateTree compilation or packaging failures.
    */
	UPROPERTY(EditAnywhere)
	FAbilityData GrantedAbility;
};
