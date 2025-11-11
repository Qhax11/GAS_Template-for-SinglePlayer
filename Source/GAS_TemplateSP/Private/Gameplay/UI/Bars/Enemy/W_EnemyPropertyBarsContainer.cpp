// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/Enemy/W_EnemyPropertyBarsContainer.h"
#include "Gameplay/UI/Bars/W_PostureBar.h"
#include "Gameplay/UI/Bars/W_HealthBar.h"

void UW_EnemyPropertyBarsContainer::InitalizePropertyBarsContainer(AActor* Owner)
{
	if (!HealthBar || !PostureBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBar or PostureBar is null in: %s"), *GetName());
		return;
	}

	HealthBar->InitializePropertyBar(Owner);
	PostureBar->InitializePropertyBar(Owner);
}
