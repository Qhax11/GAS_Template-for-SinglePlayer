// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Bars/W_ShadowGuageBar.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemGlobals.h"
#include "Kismet/KismetTextLibrary.h"

void UW_ShadowGuageBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UW_ShadowGuageBar::NativeDestruct()
{
	// Cleanup delegates
	if (CachedASC)
	{
		if (CurrentAttributeHandle.IsValid())
		{
			CachedASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).Remove(CurrentAttributeHandle);
		}
		if (MaxAttributeHandle.IsValid())
		{
			CachedASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).Remove(MaxAttributeHandle);
		}
	}

	Super::NativeDestruct();
}

void UW_ShadowGuageBar::InitializePropertyBar(AActor* Owner)
{
	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!OwnerASC)
	{
		return;
	}

	CachedASC = OwnerASC;
	BindToAttribute(OwnerASC);
}

void UW_ShadowGuageBar::PlayFadeInAnim()
{
	BP_PlayFadeInAnim();
}

void UW_ShadowGuageBar::PlayFadeOutAnim()
{
	BP_PlayFadeOutAnim();
}

void UW_ShadowGuageBar::BindToAttribute(UAbilitySystemComponent* OwnerASC)
{
	if (!OwnerASC || !CurrentAttribute.IsValid() || !MaxAttribute.IsValid())
	{
		return;
	}

	// Get initial values
	CachedCurrentValue = OwnerASC->GetNumericAttribute(CurrentAttribute);
	CachedMaxValue = OwnerASC->GetNumericAttribute(MaxAttribute);

	// Set initial UI
	SetPercentage(CachedCurrentValue, CachedMaxValue);
	SetValuesToTexts(CachedCurrentValue, CachedMaxValue);

	// Bind to attribute changes
	CurrentAttributeHandle = OwnerASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute)
		.AddUObject(this, &UW_ShadowGuageBar::OnCurrentAttributeChanged);

	MaxAttributeHandle = OwnerASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
		.AddUObject(this, &UW_ShadowGuageBar::OnMaxAttributeChanged);
}

void UW_ShadowGuageBar::OnCurrentAttributeChanged(const FOnAttributeChangeData& Data)
{
	float OldValue = CachedCurrentValue;
	CachedCurrentValue = Data.NewValue;

	SetPercentage(CachedCurrentValue, CachedMaxValue);
	SetValuesToTexts(CachedCurrentValue, CachedMaxValue);

	BP_OnAttributeChanged(CachedCurrentValue, CachedMaxValue, OldValue);
}

void UW_ShadowGuageBar::OnMaxAttributeChanged(const FOnAttributeChangeData& Data)
{
	float OldValue = CachedMaxValue;
	CachedMaxValue = Data.NewValue;

	SetPercentage(CachedCurrentValue, CachedMaxValue);
	SetValuesToTexts(CachedCurrentValue, CachedMaxValue);

	BP_OnAttributeChanged(CachedCurrentValue, CachedMaxValue, OldValue);
}

void UW_ShadowGuageBar::SetPercentage(float Value1, float Value2)
{
	if (!ProgressBar || Value2 <= 0.f)
	{
		return;
	}

	float NewValue = FMath::Clamp(Value1 / Value2, 0.f, 1.f);
	ProgressBar->SetPercent(NewValue);
}

void UW_ShadowGuageBar::SetValuesToTexts(float CurrentValue, float MaxValue)
{
	if (T_CurrentValue)
	{
		T_CurrentValue->SetText(UKismetTextLibrary::Conv_DoubleToText(
			CurrentValue, HalfToEven, false, true, 1, 324, 0, 3));
	}

	if (T_MaxValue)
	{
		T_MaxValue->SetText(UKismetTextLibrary::Conv_DoubleToText(
			MaxValue, HalfToEven, false, true, 1, 324, 0, 3));
	}
}