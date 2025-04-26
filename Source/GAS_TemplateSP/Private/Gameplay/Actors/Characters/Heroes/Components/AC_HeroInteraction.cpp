// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroInteraction.h"
#include "UI/S_UIManager.h"

UAC_HeroInteraction::UAC_HeroInteraction()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroInteraction::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s, HeroInteraction is cannot initialize"), *GetName());
		return;
	}

	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<US_UIManager>();
	if (!UIManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIManager is null in: %s"), *GetName());
		return;
	}

	TryBindInteractionInputs();
}

void UAC_HeroInteraction::TryBindInteractionInputs()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (IA_ToggleMenu)
	{
		EnhancedInputComponent->BindAction(IA_ToggleMenu, ETriggerEvent::Triggered, this, &UAC_HeroInteraction::OnToggleMenuPressed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
	}
}

void UAC_HeroInteraction::OnToggleMenuPressed()
{
	UIManager->ToggleESCMenu();
	UE_LOG(LogTemp, Warning, TEXT("TOGGLE"));
}

