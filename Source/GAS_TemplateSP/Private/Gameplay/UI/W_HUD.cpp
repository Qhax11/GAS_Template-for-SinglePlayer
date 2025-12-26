// Qhax's GAS Template for SinglePlayer


#include "Gameplay/UI/W_HUD.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/UI/Bars/W_HealthBar.h"
#include "Gameplay/UI/Bars/W_ManaBar.h"
#include "Gameplay/UI/Bars/Hero/W_HeroPropertyBarsContainer.h"

void UW_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	AGAS_CharacterBase* HeroCharacter = Cast<AGAS_CharacterBase>(GetOwningPlayerPawn());
	if (!HeroCharacter) 
	{
		return;
	}

	UAC_AbilitySet* PlayerAbiltySetComp = HeroCharacter->GetAbilitySetComponent();
	if (!PlayerAbiltySetComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySet doesen't exist in %s, so we can't bind attribute to widget"), *GetOwningPlayerPawn()->GetName());
	}

	// BeginPlay is running before AbilitySet Initialize, so we need wait
	PlayerAbiltySetComp->OnAbilitySetGiven.AddDynamic(this, &UW_HUD::OnAbilitySetGiven);
}

void UW_HUD::OnAbilitySetGiven(const AActor* OwnerActor)
{
	PropertyBarsContainer->InitalizePropertyBarsContainer(GetOwningPlayerPawn());
}
