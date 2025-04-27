// Qhax's GAS Template for SinglePlayer


#include "UI/S_UIManager.h"
#include "UI/DS_UIManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include <Kismet/GameplayStatics.h>

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

UUserWidget* US_UIManager::CreateAndShowWidget(const FWidgetData& WidgetData, APlayerController* PC)
{
	if (!WidgetData.WidgetClass.IsValid())
	{
		WidgetData.WidgetClass.LoadSynchronous();
	}

	if (!WidgetData.WidgetClass.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateAndShowWidget: Invalid WidgetClass even after load."));
		return nullptr;
	}

	UUserWidget* Widget = nullptr;
	if (PC)
	{
		Widget = CreateWidget<UUserWidget>(PC, WidgetData.WidgetClass.Get());
	}
	else
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetData.WidgetClass.Get());
	}

	if (Widget)
	{
		ApplyWidgetContextInputSettings(PC, WidgetData.Context, Widget);
		ApplyPauseBehavior(WidgetData.PauseBehavior);
		Widget->AddToViewport();
	}

	return Widget;
}

void US_UIManager::ApplyWidgetContextInputSettings(APlayerController* PC, EUIWidgetContext Context, UUserWidget* FocusedWidget)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyWidgetContextInputSettings: PlayerController is null."));
		return;
	}

	switch (Context)
	{
	case EUIWidgetContext::GameOnly:
		SetInputModeGameOnly(PC);
		SetCursorVisible(PC, false);
		break;

	case EUIWidgetContext::UIOnly:
		SetInputModeUIOnly(PC, FocusedWidget);
		SetCursorVisible(PC, true);
		break;

	case EUIWidgetContext::GameAndUI:
		SetInputModeGameAndUI(PC, FocusedWidget);
		SetCursorVisible(PC, true);
		break;

	default:
		break;
	}
}

void US_UIManager::ApplyPauseBehavior(EGamePauseBehavior PauseBehavior)
{
	if (PauseBehavior == EGamePauseBehavior::PauseGame)
	{
		SetPause(true);
	}
	else
	{
		SetPause(false);
	}
}

void US_UIManager::SetInputModeUIOnly(APlayerController* PC, UUserWidget* FocusedWidget)
{
	if (!PC)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	if (FocusedWidget)
	{
		InputMode.SetWidgetToFocus(FocusedWidget->TakeWidget());
	}
	PC->SetInputMode(InputMode);
}

void US_UIManager::SetInputModeGameOnly(APlayerController* PC)
{
	if (!PC)
	{
		return;
	}

	PC->SetInputMode(FInputModeGameOnly());
}

void US_UIManager::SetInputModeGameAndUI(APlayerController* PC, UUserWidget* FocusedWidget)
{
	if (!PC)
	{
		return;
	}

	FInputModeGameAndUI InputMode;
	if (FocusedWidget)
	{
		InputMode.SetWidgetToFocus(FocusedWidget->TakeWidget());
	}
	PC->SetInputMode(InputMode);
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
	if (bPause)
	{
		// Tam durdurmak istiyorsan:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

void US_UIManager::ToggleESCMenu()
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToggleESCMenu: PlayerController is null."));
		return;
	}

	if (!ESCMenuWidget)
	{
		if (UIManagerSettings && UIManagerSettings->ToggleMenu.WidgetClass)
		{
			ESCMenuWidget = CreateAndShowWidget(UIManagerSettings->ToggleMenu, PlayerController);
		}

		if (!ESCMenuWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("ToggleESCMenu: ESCMenuWidget is null."));
			return;
		}
	}
	else
	{
		const bool bIsMenuVisible = ESCMenuWidget->IsVisible();
		if (bIsMenuVisible)
		{
			ESCMenuWidget->SetVisibility(ESlateVisibility::Hidden);

			// Menü kapandýysa Gameplay'e geri dön
			ApplyWidgetContextInputSettings(PlayerController, EUIWidgetContext::GameOnly, nullptr);
			SetPause(false);
		}
		else
		{
			ESCMenuWidget->SetVisibility(ESlateVisibility::Visible);

			// Menü açýldýysa kendi context'ine göre ayarla
			ApplyWidgetContextInputSettings(PlayerController, UIManagerSettings->ToggleMenu.Context, ESCMenuWidget);
			SetPause(true);
		}
	}
}

