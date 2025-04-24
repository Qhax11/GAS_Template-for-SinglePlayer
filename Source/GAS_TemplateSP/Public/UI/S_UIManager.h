// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_UIManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API US_UIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateAndShowWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PC = nullptr);

	
};
