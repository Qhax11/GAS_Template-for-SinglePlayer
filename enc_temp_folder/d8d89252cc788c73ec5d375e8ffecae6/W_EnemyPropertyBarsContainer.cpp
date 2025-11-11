// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/Enemy/W_EnemyPropertyBarsContainer.h"
#include "Gameplay/UI/Bars/W_PostureBar.h"
#include "Gameplay/UI/Bars/W_HealthBar.h"

void UW_EnemyPropertyBarsContainer::InitalizePropertyBarsContainer(AActor* Owner)
{
	if (HealthBar) 
	{
		HealthBar->InitializePropertyBar(Owner);
	}

	if (PostureBar) 
	{
		PostureBar->InitializePropertyBar(Owner);
	}
}
