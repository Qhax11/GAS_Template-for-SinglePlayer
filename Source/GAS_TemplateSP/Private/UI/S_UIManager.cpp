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

	FWorldDelegates::OnWorldCleanup.AddUObject(this, &US_UIManager::OnWorldCleanup);
	SpawnDelegatesSubsystem->OnPlayerControllerSpawn.AddDynamic(this, &US_UIManager::OnPlayerControllerSpawn);
}

void US_UIManager::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	ESCMenuWidget = nullptr;
}

void US_UIManager::OnPlayerControllerSpawn(APlayerController* PC)
{
	PlayerController = PC;
}

UUserWidget* US_UIManager::CreateAndShowWidget(const FWidgetData& WidgetData, APlayerController* PC)
{
	if (WidgetData.WidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateAndShowWidget: WidgetClass is null."));
		return nullptr;
	}

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
		return;
	}

	if (!ESCMenuWidget)
	{
		ESCMenuWidget = CreateAndShowWidget(UIManagerSettings->ToggleMenu, PlayerController);
	}
	else
	{
		const bool bIsVisible = ESCMenuWidget->IsVisible();
		if (bIsVisible)
		{
			CloseMenu(ESCMenuWidget, UIManagerSettings->ToggleMenu);
		}
		else
		{
			OpenMenu(ESCMenuWidget, UIManagerSettings->ToggleMenu);
		}
	}
}

void US_UIManager::OpenMenu(UUserWidget* WidgetToOpen, const FWidgetData& WidgetData)
{
	if (!IsValid(WidgetToOpen))
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenMenu: WidgetToOpen is not valid."));
		return;
	}

	WidgetToOpen->SetVisibility(ESlateVisibility::Visible);
	ApplyWidgetContextInputSettings(PlayerController, WidgetData.Context, WidgetToOpen);
	SetPause(true);
}

void US_UIManager::CloseMenu(UUserWidget* WidgetToClose, const FWidgetData& WidgetData)
{
	if (!IsValid(WidgetToClose))
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenMenu: WidgetToOpen is not valid."));
		return;
	}

	WidgetToClose->SetVisibility(ESlateVisibility::Hidden);
	ApplyWidgetContextInputSettings(PlayerController, EUIWidgetContext::GameOnly, nullptr);
	SetPause(false);
}



