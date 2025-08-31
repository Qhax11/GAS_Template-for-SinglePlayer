// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Sound/GCN_SoundBase.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Gameplay/Tags/GAS_Tags.h"


bool AGCN_SoundBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Source || !Target)
	{
		return false;
	}

	if (GameplayCueTag.MatchesTag(GAS_Tags::TAG_GameplayCue_Sound_PlayOnSource))
	{
		PlaySoundForActor(Source);
	}
	else if (GameplayCueTag.MatchesTag(GAS_Tags::TAG_GameplayCue_Sound_PlayOnTarget))
	{
		PlaySoundForActor(Target);
	}

	return true;
}

void AGCN_SoundBase::PlaySoundForActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	if (UAC_GameplayData* DataComponent = Actor->GetComponentByClass<UAC_GameplayData>())
	{
		UDA_ActorSounds* SoundsData = DataComponent->GetActorSoundsData();
		if (SoundsData)
		{
			if (SoundsData->TagToSoundMap.Contains(GameplayCueTag))
			{
				USoundBase* AbilitySound = SoundsData->TagToSoundMap[GameplayCueTag];
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), AbilitySound, Actor->GetActorLocation());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No sound found for GameplayCueTag: %s"), *GameplayCueTag.ToString());
			}
		}
	}
}
