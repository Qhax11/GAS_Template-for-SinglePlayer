// Qhax's GAS Template for SinglePlayer


#include "UI/S_UIManager.h"
#include "UI/DS_UIManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void US_UIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

	UIManagerSettings = GetDefault<UDS_UIManager>();
	if (!UIManagerSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIManagerSettings is null in %s"), *this->GetName());
		return;
	}

	US_SpawnDelegates* SpawnDelegatesSubsystem = GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
	if (!SpawnDelegatesSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnDelegatesSubsystem is null in: %s"), *GetName());
		return;
	}

	SpawnDelegatesSubsystem->OnPlayerControllerSpawn.AddDynamic(this, &US_UIManager::OnPlayerControllerSpawn);
}

void US_UIManager::OnPlayerControllerSpawn(APlayerController* PC)
{
	PlayerController = PC;
}

UUserWidget* US_UIManager::CreateAndShowWidget(TSubclassOf<UUserWidget> WidgetClass, EUIWidgetContext WidgetContext, APlayerController* PC)
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
		switch (WidgetContext)
		{
		case EUIWidgetContext::Gameplay:
			SetInputModeGameOnly(PC);
			SetCursorVisible(PC, false);
			break;

		case EUIWidgetContext::NonGameplay:
			SetInputModeUIOnly(PC, Widget);
			SetCursorVisible(PC, true);
			break;

		case EUIWidgetContext::PauseMenu:
			SetInputModeUIOnly(PC, Widget);
			SetCursorVisible(PC, true);
			SetPause(true);
			break;
		}

		Widget->AddToViewport();
	}

	return Widget;
}

void US_UIManager::SetInputModeUIOnly(APlayerController* PC, UUserWidget* FocusedWidget)
{
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(FocusedWidget->TakeWidget());
		PC->SetInputMode(InputMode);
	}
}

void US_UIManager::SetInputModeGameOnly(APlayerController* PC)
{
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void US_UIManager::SetCursorVisible(APlayerController* PC, bool bVisible)
{
	if (PC)
	{
		PC->bShowMouseCursor = bVisible;
		PC->SetIgnoreLookInput(bVisible);
		PC->SetIgnoreMoveInput(bVisible);
	}
}

void US_UIManager::SetPause(bool bPause)
{
}

void US_UIManager::ToggleESCMenu()
{
	/*
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToggleESCMenu: PlayerController is null."));
		return;
	}

	// Eðer daha önce yaratýlmadýysa yarat
	if (!ESCMenuWidget && ESCMenuWidgetClass)
	{
		ESCMenuWidget = CreateWidget<UUserWidget>(PC, ESCMenuWidgetClass);
		if (ESCMenuWidget)
		{
			ESCMenuWidget->AddToViewport();
			ESCMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (!ESCMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToggleESCMenu: ESCMenuWidget is null."));
		return;
	}

	// Þu an görünür mü kontrolü
	if (ESCMenuWidget->IsVisible())
	{
		// Menü açýk, kapat
		ESCMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		SetInputModeGameOnly(PC);
		SetCursorVisible(PC, false);
		SetPause(false);
	}
	else
	{
		// Menü kapalý, aç
		ESCMenuWidget->SetVisibility(ESlateVisibility::Visible);
		SetInputModeUIOnly(PC, ESCMenuWidget);
		SetCursorVisible(PC, true);
		SetPause(true);
	}
	*/
}

