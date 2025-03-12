// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/HologramTargetActorBase.h"
#include "HeroHologramTargetActor.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AHeroHologramTargetActor : public AHologramTargetActorBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void Confirm() override;

	virtual void Cancel() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "HeroHologramTargetActor")
	float RotationSpeed = 5.0f;

public:
	UFUNCTION()
	void OnTargetChaned(AActor* NewTarget);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTargetChaned(AActor* NewTarget);

	UFUNCTION()
	void OnEndTargetLock();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnEndTargetLock();

private:
	void RotateToTarget(AActor* TargetActor, float DeltaTime);

	void SyncRotationWithHero(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AGAS_HeroBase* HeroBase;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USC_HeroHologramController* HeroHologramControllerComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAC_TargetLockSystem* TargetLockSystemComponent;

protected:
	virtual void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)  override;


};
