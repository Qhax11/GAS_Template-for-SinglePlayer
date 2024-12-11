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
	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentTarget;
	//TObjectPtr<AActor> CurrentTarget;
	
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
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AGAS_HeroBase* HeroBase;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USC_EyeOfView* EyeOfViewComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAC_TargetLockSystem* TargetLockSystemComponent;
};
