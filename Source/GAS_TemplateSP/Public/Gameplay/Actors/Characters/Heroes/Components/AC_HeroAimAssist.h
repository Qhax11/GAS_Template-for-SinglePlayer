// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroBase.h"
#include "AC_HeroAimAssist.generated.h"

// Handles assisted target selection and directional adjustment during attacks
// when the player is not using full target lock. This component evaluates nearby
// valid enemies and provides rotation or target hints to help attacks connect.

class UGameplayAbility;

UCLASS()
class GAS_TEMPLATESP_API UAC_HeroAimAssist : public UAC_HeroBase
{
	GENERATED_BODY()

public:
    UAC_HeroAimAssist();

    virtual void BeginPlay() override;

    void OnHeroAbilityActivated(UGameplayAbility* Ability);

    void OnHeroAbilityEnded(UGameplayAbility* Ability);

    bool IsTargetInAngle();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void StartRotation(UGameplayAbility* Ability);

    void StopRotation();

    UPROPERTY()
    UGameplayAbility* CurrentActiveAbility = nullptr;

    UPROPERTY()
    AActor* TargetActor;

    bool bIsRotating = false;

    UPROPERTY(EditAnywhere)
    float AssistMaxAngle = 45.0f; 

    UPROPERTY(EditAnywhere)
    float AssistRotationSpeed = 720.0f; 

    UPROPERTY(EditDefaultsOnly)
    class UGAS_AbilityTraceData* TraceCheckEnemy;

    UPROPERTY()
    class UAC_HeroControl* HeroController;
	
    UPROPERTY()
    class UAC_TargetLockSystem* HeroTargetLock;
};
