// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/NiagaraControllers/NiagaraController.h"

ANiagaraController::ANiagaraController()
{
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);
}

void ANiagaraController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ANiagaraController::ChaseStart, ChaseStartDelay, false);
}

void ANiagaraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!NiagaraComp || !TargetActor || !bChaseStart)
	{
		return;
	}

	NiagaraComp->SetNiagaraVariableVec3(TEXT("User_TargetLocation"), TargetActor->GetActorLocation());
}

void ANiagaraController::ChaseStart()
{
	if (!TargetActor) 
	{
		return;
	}

	NiagaraComp->SetNiagaraVariableFloat(TEXT("User_AttrationStrength"), ParticleAttractionStrength);
	bChaseStart = true;
}

