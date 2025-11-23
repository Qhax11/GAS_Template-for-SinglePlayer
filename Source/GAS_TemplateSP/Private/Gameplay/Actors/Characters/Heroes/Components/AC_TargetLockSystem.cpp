// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemGlobals.h"


UAC_TargetLockSystem::UAC_TargetLockSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_TargetLockSystem::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	if (!HeroBase || !HeroASC || !HeroTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase, HeroASC or HeroTagDelegatesComp is null in: %s)"), *GetName());
		return;
	}

	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher, EListenMode::OnAdded).BindDynamic(this, &UAC_TargetLockSystem::OnHeroFinisherTagAdded);
	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher, EListenMode::OnRemoved).BindDynamic(this, &UAC_TargetLockSystem::OnHeroFinisherTagRemoved);

	if (ensure(TracingDataStart) && ensure(TracingDataTargetChange) && ensure(TracingDataCheckForFrontActor))
	{
		TracingDataStart->Trace->bDrawEnable = bEnableTraceDebug;
		TracingDataTargetChange->Trace->bDrawEnable = bEnableTraceDebug;
		TracingDataCheckForFrontActor->Trace->bDrawEnable = bEnableTraceDebug;
		TracingDataCheckClosestTarget->Trace->bDrawEnable = bEnableTraceDebug;
	}

	if (!BindTargetLockSystemInputs()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Binding is failed in %s, cannot initialize TargetLockSystem."), *GetName());
	}

	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnEnemyDeSpawn.AddDynamic(this, &UAC_TargetLockSystem::OnEnemyDeSpawn);
	}
}

bool UAC_TargetLockSystem::BindTargetLockSystemInputs()
{
	UAC_HeroControl* HeroControlComponent = HeroBase->GetHeroControlComponent();
	if (!HeroControlComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroControlComponent is null in: %s"), *GetName());
		return false;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return false;
	}

	if (HeroControlComponent->IA_LookMouse && IA_ActivateTargetLock)
	{
		EnhancedInputComponent->BindAction(IA_ActivateTargetLock, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::ActivateTargetLock);
		EnhancedInputComponent->BindAction(HeroControlComponent->IA_LookMouse, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::LookMouse);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
		return false;
	}
}

void UAC_TargetLockSystem::ActivateTargetLock(const FInputActionValue& Value)
{
	if (!bLocked)
	{
		StartTargetLock(TracingDataStart);
	}
	else
	{
		EndTargetLock();
	}
}

void UAC_TargetLockSystem::StartTargetLock(UGAS_AbilityTraceData* TracingData)
{
 	if (!TracingData || HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher))
	{
		UE_LOG(LogTemp, Warning, TEXT("TracingData is null in: %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	TArray<AActor*> OutResultActors;
	TracingData->Trace->CreateTraceWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors);
	if (OutResultActors.IsEmpty())
	{
		return;
	}

	FilterOutDeadActors(OutResultActors);
	AActor* ClosestTarget = FindNearestActor(HeroBase, OutResultActors);

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ClosestTarget);
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetASC is null in %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	ChangeTarget(ClosestTarget, true);

	HeroASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
	HeroASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	bLocked = true;
	SetComponentTickEnabled(true);
	OnStartTargetLock.Broadcast();
}

void UAC_TargetLockSystem::FilterOutDeadActors(TArray<AActor*>& Actors)
{
	// We'll build a new array of only alive actors
	TArray<AActor*> FilteredActors;

	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			continue;
		}

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		if (!ASC)
		{
			continue;
		}

		if (!ASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
		{
			// Only keep alive actors
			FilteredActors.Add(Actor);
		}
	}

	// Replace old array with filtered version
	Actors = MoveTemp(FilteredActors);
}

void UAC_TargetLockSystem::EndTargetLock()
{
	if (!CurrentTargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTargetASC is null in %s"), *GetName());
		return;
	}

	HeroASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked, 100);
	HeroASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget, 100);
	CurrentTargetASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted);
	bLocked = false;

	/* We need target asc still for finisher, just use bLocked for checking
	CurrentTargetASC = nullptr;
	CurrentTarget = nullptr;
	*/

	OnEndTargetLock.Broadcast();

	SetComponentTickEnabled(false);
}

void UAC_TargetLockSystem::OnEnemyDeSpawn(const FCharacterDeSpawnData& EnemyDeSpawnData)
{
	if (EnemyDeSpawnData.Character != CurrentTarget)
	{
		return;
	}

	// If it's other than dead basic like finisher dead, we should ignore it.
	if (!EnemyDeSpawnData.ASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic))
	{
		return;
	}

	if (EnemyDeSpawnData.DeSpawnPhase == EDeSpawnPhase::DeathStarted)
	{
		EndTargetLock();
	}
}

void UAC_TargetLockSystem::OnHeroFinisherTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	//→ EndTargetLock() 
	// → RemoveLooseGameplayTag()
	//  → Tag delegate broadcast
	//	 → OnTargetLockedTagRemoved() (in another ability)
	// 	  → EndAbility()
	// 	   → Delegate'e write access (CRASH!) Multi-threaded access detector
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			EndTargetLock();
		});
}

void UAC_TargetLockSystem::OnHeroFinisherTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	//→ EndTargetLock() 
	// → RemoveLooseGameplayTag()
	//  → Tag delegate broadcast
	//	 → OnTargetLockedTagRemoved() (in another ability)
	// 	  → EndAbility()
	// 	   → Delegate'e write access (CRASH!) Multi-threaded access detector
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartTargetLock(TracingDataCheckClosestTarget);
		});
}

void UAC_TargetLockSystem::LookMouse(const FInputActionValue& Value)
{
	if (!bLocked || HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting) || HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher))
	{
		return;
	}

	const FVector2D VectorValue = Value.Get<FVector2D>();

	// Cooldown mechanism: Each direction can only trigger the action once per second.
	const float CurrentTime = GetWorld()->GetTimeSeconds(); 

	if ((VectorValue.X > Threshold) && (CurrentTime - TryToFindNewTargetLastExecutionTimeRight >= TryToFindNewTargetExecutionCooldown))
	{
		TryToFindNewTarget(ETargetChangeDirection::TCD_Right);
		TryToFindNewTargetLastExecutionTimeRight = CurrentTime;
	}

	if ((VectorValue.X < -Threshold) && (CurrentTime - TryToFindNewTargetLastExecutionTimeLeft >= TryToFindNewTargetExecutionCooldown))
	{
		TryToFindNewTarget(ETargetChangeDirection::TCD_Left);
		TryToFindNewTargetLastExecutionTimeLeft = CurrentTime;
	}
}

void UAC_TargetLockSystem::TryToFindNewTarget(TEnumAsByte<ETargetChangeDirection> TargetChangeDirection)
{
	if (!TracingDataTargetChange || !TracingDataCheckForFrontActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TracingDataTargetChange or TracingDataCheckForFrontActor is null in: %s"), *GetName());
		return;
	}

	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	TArray<AActor*> OutResultActors;
	TracingDataTargetChange->Trace->CreateTraceWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, OutResultActors);

	OutResultActors.Remove(CurrentTarget);

	if (OutResultActors.IsEmpty())
	{
		return;
	}

	TArray<AActor*> LeftActors;
	TArray<AActor*> RightActors;

	SplitActorsByPositionRelativeToHero(OutResultActors, LeftActors, RightActors);

	AActor* FoundNewTarget = nullptr;
	if (TargetChangeDirection == ETargetChangeDirection::TCD_Left) 
	{
		FoundNewTarget = FindNearestActor(CurrentTarget, LeftActors);
	}
	else if(TargetChangeDirection == ETargetChangeDirection::TCD_Right)
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
		FTraceRequest TraceRequest;
		TraceRequest.Direction = LookAtRotation;
		TracingDataCheckForFrontActor->Trace->CreateTraceWithTeamFilter(GetWorld(), HeroBase, ETeamAttitude::Hostile, CheckForFrontActors, TraceRequest);
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

void UAC_TargetLockSystem::ChangeTarget(AActor* NewTarget, bool bStartTargeting)
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

	if (CurrentTargetASC) 
	{
		CurrentTargetASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted);
	}
	NewTargetASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Enemy_Targeted);

	CurrentTargetASC = NewTargetASC;
	CurrentTarget = NewTarget;

	if (!bStartTargeting) 
	{
		OnTargetChanged.Broadcast(NewTarget);
	}
}

void UAC_TargetLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateCameraToTarget(DeltaTime);
	RotateHeroToTarget(DeltaTime);
}

void UAC_TargetLockSystem::RotateCameraToTarget(float DeltaTime)
{
	if (!HeroBase || !CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase or CurrentTarget null in: %s"), *GetName());
		return;
	}

	FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();
	CurrentTargetLocation.Z = CurrentTargetLocation.Z - CameraLookLocationOffsetZ;

	FRotator LookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(HeroBase->GetActorLocation(), CurrentTargetLocation);

	// Get the current camera rotation and interpolate towards the target for smooth transition
	FRotator CurrentCameraRotation = HeroBase->GetControlRotation();
	FRotator NewCameraRotation = UKismetMathLibrary::RInterpTo(CurrentCameraRotation, LookAtTargetRotation, DeltaTime, RotateInterpSpeed);
	NewCameraRotation.Pitch = RotateCameraToTargetClampPitch(NewCameraRotation.Pitch);
	NewCameraRotation.Roll = 0;
	HeroBase->GetController()->SetControlRotation(NewCameraRotation);
}

float UAC_TargetLockSystem::RotateCameraToTargetClampPitch(float NewPitch)
{
	// Normalize pitch to the range [0, 360]
	NewPitch = FMath::Fmod(NewPitch + 360.0f, 360.0f);

	// Check for looking up from below, NewPitch value is between MinPitchA and MaxPitchA
	if (NewPitch >= MinPitchA && NewPitch <= MaxPitchA)
	{
		NewPitch = FMath::Clamp(NewPitch, MinPitchA, MaxPitchA);
		return NewPitch;
	}
	// Check for looking down from above, NewPitch value is between MinPitchB and MaxPitchB
	else if (NewPitch >= MinPitchB && NewPitch <= MaxPitchB)
	{
		NewPitch = FMath::Clamp(NewPitch, MinPitchB, MaxPitchB);
		return NewPitch;
	}
	// NewPitch value is between MaxPitchA and MinPitchB
	else
	{
		float NewPitchDistanceToMaxPitchA = FMath::Abs(NewPitch - MaxPitchA);
		float NewPitchDistanceToMinPitchB = FMath::Abs(NewPitch - MinPitchB);
		// If closer to MaxPitchA degrees 
		if (NewPitchDistanceToMaxPitchA < NewPitchDistanceToMinPitchB)
		{
			NewPitch = MaxPitchA;
			return NewPitch;
		}
		// If closer to MinPitchB degrees
		else 
		{
			NewPitch = MinPitchB;
			return NewPitch;
		}
	}
}

void UAC_TargetLockSystem::RotateHeroToTarget(float DeltaTime)
{
	if (!HeroBase || !CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase or CurrentTarget null in: %s"), *GetName());
		return;
	}

	bool LockRotationTowardsTarget = HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
	bool Running = HeroASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Moving_Running);
	if (!LockRotationTowardsTarget || Running)
	{
		return;
	}

	FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();
	FRotator LookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(HeroBase->GetActorLocation(), CurrentTargetLocation);

	FRotator CurrentHeroRotation = HeroBase->GetActorRotation();
	FRotator NewHeroRotation = UKismetMathLibrary::RInterpTo(CurrentHeroRotation, LookAtTargetRotation, DeltaTime, RotateInterpSpeed);

	// Set the new rotation, but only update Yaw (Left-Right rotation), keep Pitch and Roll unchanged
	NewHeroRotation.Pitch = CurrentHeroRotation.Pitch;
	NewHeroRotation.Roll = CurrentHeroRotation.Roll;

	HeroBase->SetActorRotation(NewHeroRotation);
}



