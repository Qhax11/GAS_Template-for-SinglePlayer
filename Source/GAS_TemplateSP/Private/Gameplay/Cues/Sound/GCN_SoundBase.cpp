// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Sound/GCN_SoundBase.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "Gameplay/Data/DA_ActorSounds.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Gameplay/Tags/GAS_Tags.h"


void AGCN_SoundBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Source || !Target)
	{
		return;
	}

	// Diyelim GameplayCueTag sadece bir tane
	bool bIsSourceTag = GameplayCueTag.MatchesTag(GAS_Tags::TAG_GameplayCue_Sound_PlayOnSource);
	bool bIsTargetTag = GameplayCueTag.MatchesTag(GAS_Tags::TAG_GameplayCue_Sound_PlayOnTarget);

	if (bIsSourceTag)
	{
		if (UAC_GameplayData* DataComponent = Source->GetComponentByClass<UAC_GameplayData>())
		{
			UDA_ActorSounds* SoundsData = DataComponent->GetActorSoundsData();
			if (SoundsData)
			{
				if (SoundsData->TagToSoundMap.Contains(GameplayCueTag))
				{
					USoundBase* AbilitySound = SoundsData->TagToSoundMap[GameplayCueTag];
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), AbilitySound, Target->GetActorLocation());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No sound found for GameplayCueTag: %s"), *GameplayCueTag.ToString());
				}
			}
		}
	}
	else if (bIsTargetTag)
	{
		if (UAC_GameplayData* DataComponent = Target->GetComponentByClass<UAC_GameplayData>())
		{
			UDA_ActorSounds* SoundsData = DataComponent->GetActorSoundsData();
			if (SoundsData)
			{
				if (SoundsData->TagToSoundMap.Contains(GameplayCueTag))
				{
					USoundBase* AbilitySound = SoundsData->TagToSoundMap[GameplayCueTag];
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), AbilitySound, Target->GetActorLocation());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No sound found for GameplayCueTag: %s"), *GameplayCueTag.ToString());
				}
			}
		}
	}




}
