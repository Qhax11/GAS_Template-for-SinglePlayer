// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/Components/WC_PropertyBarsContainer.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/UI/Bars/W_PropertyBarsContainer.h"


void UWC_PropertyBarsContainer::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is null in %s, cannot initialize PropertyBar."), *this->GetName());
		return;
	}

	// BeginPlay is running before AbilitySet Initialize, so we need wait
	if (UAC_AbilitySet* AbiltySetComp = GetOwner()->GetComponentByClass<UAC_AbilitySet>()) 
	{
		AbiltySetComp->OnAbilitySetGiven.AddDynamic(this, &UWC_PropertyBarsContainer::OnAbilitySetGiven);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySet doesen't exist in %s, so we can't bind attribute to widget"), *GetOwner()->GetName());
	}
}

void UWC_PropertyBarsContainer::OnAbilitySetGiven(const AActor* OwnerActor)
{
	PropertyBarsContainer = Cast<UW_PropertyBarsContainer>(GetUserWidgetObject());
	if (PropertyBarsContainer && OwnerActor)
	{
		PropertyBarsContainer->InitalizePropertyBarsContainer(GetOwner());
	}
}
