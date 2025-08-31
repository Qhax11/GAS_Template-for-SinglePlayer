// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AC_GameplayData.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UDA_ActorSounds : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Sounds")
    TMap<FGameplayTag, USoundBase*> TagToSoundMap;

};

// Defines a mapping between an attack type (performed by the source) and the resulting camera shake intensity.
USTRUCT(BlueprintType)
struct FAttackTypeCameraShakePair
{
    GENERATED_BODY()

    // The type tag of the attack performed by the source (e.g., Light, Heavy, Special)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.Type"))
    FGameplayTag AttackTypeTag;

    // The intensity of the camera shake applied when this attack type is executed.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CameraShakeForce = 0.f;
};

/**
 * Each character defines the camera shake forces that should be applied for their different attack types.
 * This allows data-driven tuning of shake intensity per attack per character.
 *
 * Camera shakes are triggered via a HeroGameplayCamera with cue and are applied based on the actor executing the effect (the source).
 */
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UDA_AttackTypeToCameraShake : public UDataAsset
{
    GENERATED_BODY()

public:
    // List of attack type → camera shake mappings.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
    TArray<FAttackTypeCameraShakePair> AttackTypeCameraShakeMappings;

    // Returns the camera shake force for the given attack type tag.
    // If the type is not found, returns 0.
    float FindCameraShakeForce(FGameplayTag AttackTypeTag) const
    {
        for (const FAttackTypeCameraShakePair& Pair : AttackTypeCameraShakeMappings)
        {
            if (Pair.AttackTypeTag == AttackTypeTag)
            {
                return Pair.CameraShakeForce;
            }
        }
        return 0.f;
    }
};

USTRUCT(BlueprintType)
struct FAttackTypeCameraZoomPair
{
    GENERATED_BODY()

    // The type tag of the attack performed by the source (e.g., Light, Heavy, Special)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.Type"))
    FGameplayTag AttackTypeTag;

    // Zoom amount or FOV change applied for this attack type
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ZoomAmount = 0.f;

    // Optional: zoom duration
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ZoomDuration = 0.25f;
};

/**
 * Each character defines the camera zoom amounts that should be applied for their different attack types.
 * This allows data-driven tuning of zoom intensity and duration per attack per character.
 *
 * Camera zooms are triggered via a HeroGameplayCamera with cue and are applied based on the actor executing the effect (the source).
 */
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UDA_AttackTypeToCameraZoom : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
    TArray<FAttackTypeCameraZoomPair> AttackTypeCameraZoomMappings;

    float FindZoomAmount(FGameplayTag AttackTypeTag) const
    {
        for (const FAttackTypeCameraZoomPair& Pair : AttackTypeCameraZoomMappings)
        {
            if (Pair.AttackTypeTag == AttackTypeTag)
            {
                return Pair.ZoomAmount;
            }
        }
        return 0.f;
    }

    float FindZoomDuration(FGameplayTag AttackTypeTag) const
    {
        for (const FAttackTypeCameraZoomPair& Pair : AttackTypeCameraZoomMappings)
        {
            if (Pair.AttackTypeTag == AttackTypeTag)
            {
                return Pair.ZoomDuration;
            }
        }
        return 0.25f;
    }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TEMPLATESP_API UAC_GameplayData : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_GameplayData();
	
	UFUNCTION(BlueprintCallable)
	UDA_ActorSounds* GetActorSoundsData();

	UFUNCTION(BlueprintCallable)
	UDA_AttackTypeToCameraShake* GetAttackTypeToCameraShakeData();

    UFUNCTION(BlueprintCallable)
    UDA_AttackTypeToCameraZoom* GetAttackTypeToCameraZoomData();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	TObjectPtr<UDA_ActorSounds> DA_ActorSounds;

	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	TObjectPtr<UDA_AttackTypeToCameraShake> DA_AttackTypeToCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
    TObjectPtr<UDA_AttackTypeToCameraZoom> DA_AttackTypeToCameraZoom;

};
