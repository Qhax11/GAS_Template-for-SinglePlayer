// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_AbilitySet.h"
#include "LevelManager/S_LevelManager.h"

UAC_AbilitySet::UAC_AbilitySet()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_AbilitySet::Initialize(UGAS_AbilitySystemComponent* ASC)
{
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC is null in: %s. AbilitySet cannot be initialized."), *GetName());
		return;
	}

	if (!AbilitySet)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySet is null in: %s. AbilitySet cannot be initialized."), *GetName());
		return;
	}

	US_LevelManager* LevelManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_LevelManager>();
	if (!LevelManagerSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSubsystem is null in: %s"), *GetName());
		return;
	}

	if (AbilitySetTutorial && LevelManagerSubsystem->IsCurrentLevel(FName("Tutorial")))
	{
		if (ASC->GiveAbilitySet(AbilitySetTutorial))
		{
			OnAbilitySetGiven.Broadcast(GetOwner());
		}
	}
	else 
	{
		if (ASC->GiveAbilitySet(AbilitySet))
		{
			OnAbilitySetGiven.Broadcast(GetOwner());
		}
	}
}






