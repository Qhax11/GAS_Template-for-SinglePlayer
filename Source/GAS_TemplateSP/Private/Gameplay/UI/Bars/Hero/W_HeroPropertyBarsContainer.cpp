// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/Hero/W_HeroPropertyBarsContainer.h"
#include "Gameplay/UI/Bars/W_PostureBar.h"
#include "Gameplay/UI/Bars/W_HealthBar.h"
#include "Gameplay/UI/Bars/W_ShadowGuageBar.h"


void UW_HeroPropertyBarsContainer::InitalizePropertyBarsContainer(AActor* Owner)
{
	if (!HealthBar || !PostureBar || !ShadowGaugeBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBar or PostureBar or ShadowGaugeBar is null in: %s"), *GetName());
		return;
	}

	HealthBar->InitializePropertyBar(Owner);
	PostureBar->InitializePropertyBar(Owner);
	ShadowGaugeBar->InitializePropertyBar(Owner);
}
