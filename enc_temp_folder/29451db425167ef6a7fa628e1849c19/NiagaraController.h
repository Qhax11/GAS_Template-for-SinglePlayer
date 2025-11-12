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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NiagaraController")
	UNiagaraComponent* NiagaraComp;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	AActor* TargetActor;

	// Minimum distance to stop
	UPROPERTY(EditAnywhere, Category = "NiagaraController")
	float StopDistance = 10.f;

	// Header'a ekle
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ActorMaxSpeed = 2000.f; // Actor'ün max hýzý

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ActorAcceleration = 3000.f; // Actor'ün ivmesi

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ParticleAttractionStrength = 500.f; // Partikül çekim gücü

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ParticleDrag = 2.f; // Partikül drag'ý

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ChaseStartDelay = 1.f;


private:
	FTimerHandle TimerHandle;
	bool bChaseStart = false;
private:
	FVector Velocity = FVector::ZeroVector;
};
