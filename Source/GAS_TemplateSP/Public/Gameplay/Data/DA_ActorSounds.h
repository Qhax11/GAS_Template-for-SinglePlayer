// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "DA_ActorSounds.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UDA_ActorSounds : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TMap<FGameplayTag, USoundBase*> TagToSoundMap;
	
};
