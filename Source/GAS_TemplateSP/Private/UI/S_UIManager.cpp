// Qhax's GAS Template for SinglePlayer


#include "UI/S_UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UUserWidget* US_UIManager::CreateAndShowWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PC)
{
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateAndShowWidget: Invalid params"));
		return nullptr;
	}

	UUserWidget* Widget = nullptr;
	if (PC) 
	{
		Widget = CreateWidget<UUserWidget>(PC, WidgetClass);
	}
	else
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}

	if (Widget)
	{
		Widget->AddToViewport();
	}

	return Widget;
}

