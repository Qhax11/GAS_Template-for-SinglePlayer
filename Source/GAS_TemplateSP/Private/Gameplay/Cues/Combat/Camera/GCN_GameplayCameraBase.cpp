// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/Camera/GCN_GameplayCameraBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

bool AGCN_GameplayCameraBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Super::OnExecuted(Source, Target, Parameters))
	{
		return false;
	}

	// Check Source
	if(SourceASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Hero))
	{
		AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(Source);
		HeroGameplayCameraComponent = Hero->GetHeroGameplayCameraComponent();
		return true;
	}

	// Check Target
	if (TargetASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Hero))
	{
		AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(Target);
		HeroGameplayCameraComponent = Hero->GetHeroGameplayCameraComponent();
		return true;
	}

	return false;
}
