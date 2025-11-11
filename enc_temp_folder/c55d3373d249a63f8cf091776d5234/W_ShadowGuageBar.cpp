// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/W_ShadowGuageBar.h"
#include "Gameplay/Attributes/AS_Hero.h"

void UW_ShadowGuageBar::SetDefaultValues(UAbilitySystemComponent* OwnerASC)
{
	if (UAS_Base* CharacterAttributes = const_cast<UAS_Base*>(OwnerASC->GetSet<UAS_Base>()))
	{
		SetPercantage(CharacterAttributes->GetPosture(), CharacterAttributes->GetMaxPosture());
		SetValuesToTexts(CharacterAttributes->GetPosture(), CharacterAttributes->GetMaxPosture());
	}
}

void UW_ShadowGuageBar::BindAttributes(UAbilitySystemComponent* OwnerASC)
{
	if (UAS_Hero* CharacterAttributes = const_cast<UAS_Hero*>(OwnerASC->GetSet<UAS_Hero>()))
	{
		CharacterAttributes->OnShadowGaugeChanged.AddDynamic(this, &UW_ShadowGuageBar::ShadowGuageChanged);
		CharacterAttributes->OnMaxShadowGaugeChanged.AddDynamic(this, &UW_ShadowGuageBar::MaxShadowGuageChanged);
	}
}

void UW_ShadowGuageBar::ShadowGuageChanged(const FAttributeChangeCallbackData& Data)
{
	SetPercantage(Data.CurrentValue, Data.MaxValue);
	SetValuesToTexts(Data.CurrentValue, Data.MaxValue);
}

void UW_ShadowGuageBar::MaxShadowGuageChanged(const FAttributeChangeCallbackData& Data)
{
	SetPercantage(Data.CurrentValue, Data.MaxValue);
	SetValuesToTexts(Data.CurrentValue, Data.MaxValue);
}

