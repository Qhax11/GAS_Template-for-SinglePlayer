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

	/*
	FVector CurrentPos = NiagaraComp->GetComponentLocation();
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector Dir = (TargetPos - CurrentPos);
	float Dist = Dir.Size();

	if (Dist <= StopDistance)
	{
		// Hedefe yaklaştı → dur
		NiagaraComp->SetNiagaraVariableVec3(TEXT("User_TargetLocation"), CurrentPos);
	}
	else
	{
		// Hedefe doğru hareket
		FVector MoveStep = Dir.GetSafeNormal() * MoveSpeed * DeltaTime;
		FVector NewPos = CurrentPos + MoveStep;
	}
	*/
}

void ANiagaraController::ChaseStart()
{
	NiagaraComp->SetNiagaraVariableFloat(TEXT("User.Drag"), ChaseDrag);
	bChaseStart = true;
}

