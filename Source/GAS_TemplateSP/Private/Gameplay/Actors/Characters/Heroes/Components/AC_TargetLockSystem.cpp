// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemGlobals.h"

UAC_TargetLockSystem::UAC_TargetLockSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
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
	TracingDataStart->Trace->CreateTraceFromTargetingDataWithTeamFilter(GetWorld(), OutResultActors, HeroBase, ETeamAttitude::Hostile);

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
}

void UAC_TargetLockSystem::LookMouse(const FInputActionValue& Value)
{
	const FVector2D VectorValue = Value.Get<FVector2D>();

	// Cooldown mechanism: Each direction can only trigger the action once per second.
	const float CurrentTime = GetWorld()->GetTimeSeconds(); 

	static float LastExecutionTimeRight = 0.0f; 
	static float LastExecutionTimeLeft = 0.0f;  

	if (VectorValue.X > Threshold && CurrentTime - LastExecutionTimeRight >= 1.0f)
	{
		TryToChangeTarget(ETargetChangeDirection::Right);
		LastExecutionTimeRight = CurrentTime; 
	}

	if (VectorValue.X < -Threshold && CurrentTime - LastExecutionTimeLeft >= 1.0f)
	{
		TryToChangeTarget(ETargetChangeDirection::Left);
		LastExecutionTimeLeft = CurrentTime; 
	}
}

void UAC_TargetLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_TargetLockSystem::TryToChangeTarget(TEnumAsByte<ETargetChangeDirection> TargetChangeDirection)
{
	TArray<AActor*> OutResultActors;
	if (TargetChangeDirection == ETargetChangeDirection::Left) 
	{
		TracingDataLeft->Trace->CreateTraceFromTargetingDataWithTeamFilter(GetWorld(), OutResultActors, HeroBase, ETeamAttitude::Hostile);
	}
	else 
	{
		TracingDataRight->Trace->CreateTraceFromTargetingDataWithTeamFilter(GetWorld(), OutResultActors, HeroBase, ETeamAttitude::Hostile);
	}

	if (!OutResultActors.IsValidIndex(0))
	{
		return;
	}

	if (OutResultActors[0] == CurrentTarget)
	{
		return;
	}

	UAbilitySystemComponent* NewTargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OutResultActors[0]);
	if (!NewTargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewTargetASC is null in %s, cannot initialize TargetLockSystem."), *GetName());
		return;
	}

	if (CurrentTargetASC) 
	{
		CurrentTargetASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);
	}

	NewTargetASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Enemy_Targeted);
	CurrentTargetASC = NewTargetASC;
	CurrentTarget = OutResultActors[0];
}



