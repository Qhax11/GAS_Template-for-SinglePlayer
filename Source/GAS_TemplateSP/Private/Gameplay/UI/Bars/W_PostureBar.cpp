// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/W_PostureBar.h"

void UW_PostureBar::SetDefaultValues(UAbilitySystemComponent* OwnerASC)
{
	if (UAS_Base* CharacterAttributes = const_cast<UAS_Base*>(OwnerASC->GetSet<UAS_Base>()))
	{
		SetPercantage(CharacterAttributes->GetPosture(), CharacterAttributes->GetMaxPosture());
		SetValuesToTexts(CharacterAttributes->GetPosture(), CharacterAttributes->GetMaxPosture());
	}
}

void UW_PostureBar::BindAttributes(UAbilitySystemComponent* OwnerASC)
{
	if (UAS_Base* CharacterAttributes = const_cast<UAS_Base*>(OwnerASC->GetSet<UAS_Base>()))
	{
		CharacterAttributes->OnPostureChanged.AddDynamic(this, &UW_PostureBar::PostureChanged);
		CharacterAttributes->OnMaxPostureChanged.AddDynamic(this, &UW_PostureBar::MaxPostureChanged);
	}
}

void UW_PostureBar::PostureChanged(const FAttributeChangeCallbackData& Data)
{
	SetPercantage(Data.CurrentValue, Data.MaxValue);
	SetValuesToTexts(Data.CurrentValue, Data.MaxValue);
	BP_OnPostureChanged(Data);
}

void UW_PostureBar::MaxPostureChanged(const FAttributeChangeCallbackData& Data)
{
	SetPercantage(Data.CurrentValue, Data.MaxValue);
	SetValuesToTexts(Data.CurrentValue, Data.MaxValue);
	BP_OnPostureChanged(Data);
}
