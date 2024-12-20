// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "HologramTargetActorBase.generated.h"

UENUM(BlueprintType)
enum EHologramDirectionToTarget : uint8
{
	HDT_None,
	HDT_Left,
	HDT_Right,
	HDT_Forward,
	HDT_Backward
};

UCLASS()
class GAS_TEMPLATESP_API AHologramTargetActorBase : public AGAS_TargetActorBase
{
	GENERATED_BODY()

public:
	AHologramTargetActorBase();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void UpdateRelativeDirectionToTarget();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAttackMontageFromRelativePositionToTarget();

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EHologramDirectionToTarget>, UAnimMontage*> DirectionalAttackMontage;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* AttackMontage;

protected:

	void OnDirectionChanged(EHologramDirectionToTarget NewDirection);

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EHologramDirectionToTarget> LastDirectionToTarget = EHologramDirectionToTarget::HDT_None;

public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

};
