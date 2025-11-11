// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraController.generated.h"

UCLASS()
class GAS_TEMPLATESP_API ANiagaraController : public AActor
{
	GENERATED_BODY()
	
public:	
	ANiagaraController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void ChaseStart();

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	UNiagaraComponent* NiagaraComp;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	AActor* TargetActor;

	// Minimum distance to stop
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float StopDistance = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ChaseDrag = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ChaseStartDelay = 2.f;

private:
	FTimerHandle TimerHandle;
	bool bChaseStart = false;
};
