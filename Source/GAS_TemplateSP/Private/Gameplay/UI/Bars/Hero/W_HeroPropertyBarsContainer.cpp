// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/Hero/W_HeroPropertyBarsContainer.h"
#include "Gameplay/UI/Bars/W_PostureBar.h"
#include "Gameplay/UI/Bars/W_HealthBar.h"

void UW_HeroPropertyBarsContainer::InitalizePropertyBarsContainer(AActor* Owner)
{
	HealthBar->InitializePropertyBar(Owner);
	PostureBar->InitializePropertyBar(Owner);
}
