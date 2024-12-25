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

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Checks if the direction has changed.
	UFUNCTION(BlueprintCallable)
	bool UpdateRelativeDirectionToTarget();

	UPROPERTY(EditDefaultsOnly, Category = "AHologramTargetActorBase")
	TMap<TEnumAsByte<EHologramDirectionToTarget>, UAnimMontage*> DirectionalAttackMontages;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* AttackMontage;

protected:

	void OnDirectionToTargetChanged(EHologramDirectionToTarget NewDirection);

	void UptadeAttackMontageFromRelativePositionToTarget();

	UAnimMontage* GetAttackMontageFromRelativePositionToTarget();

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EHologramDirectionToTarget> LastDirectionToTarget = EHologramDirectionToTarget::HDT_None;

public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* EnemyDetectionSphere;

protected:

	bool bIsTargetInRange;

	UFUNCTION()
	virtual void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PlayMontageWithCallback(UAnimMontage* MontageToPlay);

	UFUNCTION()
	void OnPlayMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	class UAnimInstance* AnimInstance;
};
