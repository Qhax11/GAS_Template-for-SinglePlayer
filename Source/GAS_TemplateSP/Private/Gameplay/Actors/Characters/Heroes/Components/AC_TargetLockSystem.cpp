// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemGlobals.h"

UAC_TargetLockSystem::UAC_TargetLockSystem()
{
	SetComponentTickEnabled(false);
}

void UAC_TargetLockSystem::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s)"), *GetName());
		return;
	}

	HeroASC = HeroBase->GetAbilitySystemComponent();
	if (!HeroASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroASC is null in %s, cannot initialize HeroControl."), *this->GetName());
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	TryBindTargetLockSystemInputs(EnhancedInputComponent);

	if (TracingDataStart && TracingDataTargetChange && TracingDataCheckForFrontActor)
	{
		TracingDataStart->Trace->bDrawEnable = bEnableTraceDebug;
		TracingDataTargetChange->Trace->bDrawEnable = bEnableTraceDebug;
		TracingDataCheckForFrontActor->Trace->bDrawEnable = bEnableTraceDebug;
	}
}

void UAC_TargetLockSystem::TryBindTargetLockSystemInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (ActivateTargetLockInput && LookMouseInput)
	{
		EnhancedInputComponent->BindAction(ActivateTargetLockInput, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::TryActivateTargetLock);
		EnhancedInputComponent->BindAction(LookMouseInput, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::LookMouse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
	}
}

void UAC_TargetLockSystem::TryActivateTargetLock(const FInputActionValue& Value)
{
	if (!bLocked)
	{
		StartTargetLock();
	}
	else
	{
		EndTargetLock();
	}
}

void UAC_TargetLockSystem::StartTargetLock()
{
	if (!TracingDataStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetingData is null in: %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	TArray<AActor*> OutResultActors;
	TracingDataStart->Trace->CreateTraceFromTraceDataWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors);

	if (!OutResultActors.IsValidIndex(0))
	{
		return;
	}

	CurrentTargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OutResultActors[0]);
	if (!CurrentTargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetASC is null in %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	HeroASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Hero_TargetLocked);
	CurrentTargetASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);
	CurrentTarget = OutResultActors[0];
	bLocked = true;

	SetComponentTickEnabled(true);
}

void UAC_TargetLockSystem::EndTargetLock()
{
	if (!CurrentTargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerASC is null in %s, cannot initialize ability slots."), *GetName());
		return;
	}

	HeroASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Hero_TargetLocked);
	CurrentTargetASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);
	CurrentTarget = nullptr;
	bLocked = false;

	SetComponentTickEnabled(false);
}

void UAC_TargetLockSystem::LookMouse(const FInputActionValue& Value)
{
	if (!bLocked)
	{
		return;
	}

	const FVector2D VectorValue = Value.Get<FVector2D>();

	// Cooldown mechanism: Each direction can only trigger the action once per second.
	const float CurrentTime = GetWorld()->GetTimeSeconds(); 

	if (VectorValue.X > Threshold && CurrentTime - LastExecutionTimeRight >= ExecutionCooldownHorizontal)
	{
		TryToFindNewTarget(ETargetChangeDirection::Right);
		LastExecutionTimeRight = CurrentTime; 
	}

	if (VectorValue.X < -Threshold && CurrentTime - LastExecutionTimeLeft >= ExecutionCooldownHorizontal)
	{
		TryToFindNewTarget(ETargetChangeDirection::Left);
		LastExecutionTimeLeft = CurrentTime; 
	}
}

void UAC_TargetLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_TargetLockSystem::TryToFindNewTarget(TEnumAsByte<ETargetChangeDirection> TargetChangeDirection)
{
	if (!TracingDataTargetChange || !TracingDataCheckForFrontActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TracingDataTargetChange or  TracingDataCheckForFrontActor is null in: %s"), *GetName());
		return;
	}

	TArray<AActor*> OutResultActors;
	TracingDataTargetChange->Trace->CreateTraceFromTraceDataWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors);

	OutResultActors.Remove(CurrentTarget);

	if (OutResultActors.IsEmpty())
	{
		return;
	}

	TArray<AActor*> LeftActors;
	TArray<AActor*> RightActors;

	SplitActorsByPositionRelativeToHero(OutResultActors, LeftActors, RightActors);

	AActor* FoundNewTarget = nullptr;
	if (TargetChangeDirection == ETargetChangeDirection::Left) 
	{
		FoundNewTarget = FindNearestActor(CurrentTarget, LeftActors);
	}
	else if(TargetChangeDirection == ETargetChangeDirection::Right)
	{
		FoundNewTarget = FindNearestActor(CurrentTarget, RightActors);
	}

	// If there is an enemy directly in the player's line of sight (viewing direction), we select it as the new target
    // even if it's further away than the current target. This prioritizes enemies in front of the player,
    // ensuring a more dynamic target selection based on the player's perspective.
	if (FoundNewTarget)
	{
		TArray<AActor*> CheckForFrontActors;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeroBase->GetActorLocation(), FoundNewTarget->GetActorLocation());
		TracingDataCheckForFrontActor->Trace->CreateTraceFromTraceDataWithTeamFilterWithDirection(GetWorld(), HeroBase, ETeamAttitude::Hostile, LookAtRotation, CheckForFrontActors);
		if (CheckForFrontActors.IsValidIndex(0)) 
		{
			// If the actor in front of the player is different from the current target, set it as the new target.
			if (CheckForFrontActors[0] != CurrentTarget) 
			{
				FoundNewTarget = CheckForFrontActors[0];
			}
		}
	}
	
	ChangeTarget(FoundNewTarget);
}

void UAC_TargetLockSystem::SplitActorsByPositionRelativeToHero(const TArray<AActor*>& InActors, TArray<AActor*>& OutLeftActors, TArray<AActor*>& OutRightActors)
{
	FVector HeroLocation = HeroBase->GetActorLocation();
	FVector HeroForward = HeroBase->GetActorForwardVector();

	for (AActor* Actor : InActors)
	{
		FVector TargetLocation = Actor->GetActorLocation();
		FVector ToTarget = TargetLocation - HeroLocation;

		FVector CrossProductResult = FVector::CrossProduct(HeroForward, ToTarget);

		if (CrossProductResult.Z > 0)
		{
			OutRightActors.Add(Actor);
		}
		else if (CrossProductResult.Z <= 0)
		{
			OutLeftActors.Add(Actor);
		}
	}
}

AActor* UAC_TargetLockSystem::FindNearestActor(AActor* TargetedActor, TArray<AActor*> ActorArray)
{
	if (ActorArray.IsEmpty())
	{
		return nullptr;
	}

	AActor* NearestActor = nullptr;
	float NearestDistance = FLT_MAX;

	for (AActor* Actor : ActorArray)
	{
		float Distance = FVector::Dist(TargetedActor->GetActorLocation(), Actor->GetActorLocation());

		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestActor = Actor;
		}
	}

	return NearestActor;
}

void UAC_TargetLockSystem::ChangeTarget(AActor* NewTarget)
{
	if (!NewTarget) 
	{
		UE_LOG(LogTemp, Warning, TEXT("NewTarget is null in %s, cannot ChangeTarget"), *GetName());
		return;
	}

	UAbilitySystemComponent* NewTargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(NewTarget);
	if (!NewTargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewTargetASC is null in %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	CurrentTargetASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);
	NewTargetASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);

	CurrentTargetASC = NewTargetASC;
	CurrentTarget = NewTarget;
}





