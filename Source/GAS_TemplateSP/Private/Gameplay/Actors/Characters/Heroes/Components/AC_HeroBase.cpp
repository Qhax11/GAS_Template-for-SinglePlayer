// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

UAC_HeroBase::UAC_HeroBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroBase::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s, cannot initialize in BeginPlay()!"), *GetName());
		return;
	}

	HeroASC = HeroBase->GetAbilitySystemComponent();
	HeroTagDelegatesComp = HeroBase->GetTagDelegatesComponent();
}


