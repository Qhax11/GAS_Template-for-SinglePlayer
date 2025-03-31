// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "AbilitySystemGlobals.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "ShadowTargetActorBase.generated.h"

UENUM(BlueprintType)
enum EShadowDirectionToTarget : uint8
{
	HDT_None,
	HDT_Left,
	HDT_Right,
	HDT_Forward,
	HDT_Backward
};

UCLASS()
class GAS_TEMPLATESP_API AShadowTargetActorBase : public AGAS_TargetActorBase
{
	GENERATED_BODY()

protected:
	AShadowTargetActorBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void Confirm() override;

	virtual void Cancel() override;

	UPROPERTY(EditDefaultsOnly, Category = "HeroHologramTargetActor")
	float RotationSpeed = 5.0f;

	void RotateToTarget(AActor* TargetActor, float DeltaTime);

	// Checks if the direction has changed.
	UFUNCTION(BlueprintCallable)
	bool UpdateRelativeDirectionToTarget();

	UPROPERTY(EditDefaultsOnly, Category = "ShadowTargetActorBase")
	TArray<TSubclassOf<UGA_MeleeAttackBase>> ShadowAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "AHologramTargetActorBase")
	TMap<TEnumAsByte<EShadowDirectionToTarget>, TSubclassOf<UGA_MeleeAttackBase>> DirectionalAttackAbilities;

public:
	void SetCurrentTarget(AActor* NewCurrentTarget);

	AActor* GetCurrentTarget();

	TSubclassOf<UGA_MeleeAttackBase> GetSelectedShadowAbility();

protected:
	TSubclassOf<UGA_MeleeAttackBase> SelectedShadowAbility;

	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CurrentTarget;

	class AGAS_CharacterBase* InstigatorCharacter;

	void OnDirectionToTargetChanged(EShadowDirectionToTarget NewDirection);

	void UptadeAttackMontageFromRelativePositionToTarget();

	UGA_MeleeAttackBase* GetAttackAbilityFromRelativePositionToTarget();

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EShadowDirectionToTarget> LastDirectionToTarget = EShadowDirectionToTarget::HDT_None;

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
