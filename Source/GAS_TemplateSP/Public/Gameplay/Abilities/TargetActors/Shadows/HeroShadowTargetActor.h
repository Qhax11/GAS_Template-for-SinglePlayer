// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/Shadows/ShadowTargetActorBase.h"
#include "HeroShadowTargetActor.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AHeroShadowTargetActor : public AShadowTargetActorBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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
	void SyncRotationWithHero(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AGAS_HeroBase* HeroBase;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USC_HeroShadowController* HeroShadowControllerComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAC_TargetLockSystem* TargetLockSystemComponent;

protected:
	virtual void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)  override;


};
