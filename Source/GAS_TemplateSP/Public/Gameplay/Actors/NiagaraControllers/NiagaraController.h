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

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ParticleAttractionStrength = 4000.f; // Partikül çekim gücü

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float FalloffExponent = 0.3f; // Partikül çekim gücü

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ParticleDrag = 10.f; // Partikül drag'ý

	UPROPERTY(EditDefaultsOnly, Category = "NiagaraController")
	float ChaseStartDelay = 0.5f;

private:
	FTimerHandle TimerHandle;
	bool bChaseStart = false;

};
