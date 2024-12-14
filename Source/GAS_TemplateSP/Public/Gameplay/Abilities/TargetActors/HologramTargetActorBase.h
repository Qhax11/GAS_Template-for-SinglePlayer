// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "HologramTargetActorBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API AHologramTargetActorBase : public AGAS_TargetActorBase
{
	GENERATED_BODY()

public:

	AHologramTargetActorBase();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

};
