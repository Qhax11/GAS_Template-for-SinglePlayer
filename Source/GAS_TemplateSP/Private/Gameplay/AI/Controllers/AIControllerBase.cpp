// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/Components/AC_Team.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/AI/Components/AC_StateManager.h"
#include <Kismet/KismetMathLibrary.h>
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"


AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("AISenseConfig_SightComponent"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 90.f;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("PerceptionComponent"));
	PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	PerceptionComponent->SetDominantSense(AISenseConfig_Sight->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::TargetPreceptionUpdated);

	StateTreeAIComponent = CreateDefaultSubobject<UST_Base>(TEXT("StateTreeaAIComponent"));

	BehaviorDecisionComponent = CreateDefaultSubobject<UAC_BehaviorDecision>(TEXT("BehaviorDecisionComponent"));
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	ControlledEnemy = Cast<AGAS_EnemyBase>(GetPawn());
	if (!ControlledEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledCharacter is null in: %s, Controller can not initialize"), *GetName());
		return;
	}

	if (ControlledEnemy->GetAbilitySystemComponent()) 
	{
		ControlledEnemyASC = ControlledEnemy->GetAbilitySystemComponent();
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TargetHero = Cast<AGAS_HeroBase>(PlayerPawn);
	if (!TargetHero)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetHero is null in: %s, can not initialize"), *GetName());
		return;
	}

	if (UCrowdFollowingComponent* CrowdComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality) 
		{
		case 1: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		CrowdComponent->SetAvoidanceGroup(1);
		CrowdComponent->SetGroupsToAvoid(1);
		CrowdComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotationTowardsTarget(DeltaTime);
}

void AAIControllerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
}

void AAIControllerBase::UpdateRotationTowardsTarget(float DeltaTime)
{
	if (!ControlledEnemyASC || !ControlledEnemy || !TargetHero)
	{
		return;
	}

	if (!ControlledEnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Rotation_LockTowardsTarget))
	{
		return;
	}

	FVector PlayerLocation = TargetHero->GetActorLocation();
	FVector Direction = (PlayerLocation - ControlledEnemy->GetActorLocation()).GetSafeNormal();
	FRotator TargetRot = Direction.Rotation();

	FRotator CurrentRot = ControlledEnemy->GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotationSpeed);
	ControlledEnemy->SetActorRotation(FRotator(CurrentRot.Pitch, NewRot.Yaw, CurrentRot.Roll));
}

void AAIControllerBase::TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor && !bHasTargetBeenDetected)
	{
		AGAS_CharacterBase* TargetCharacter = Cast<AGAS_CharacterBase>(Actor);
		if (!TargetCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Detected target is not AGAS_CharacterBase in: %s"), *GetName());
			return;
		}

		if (!StateTreeAIComponent) 
		{
			UE_LOG(LogTemp, Warning, TEXT("StateTreeAIComponent is null in: %s"), *GetName());
			return;
		}

		OnTargetDetected.Broadcast(Actor);
		bHasTargetBeenDetected = true;
	}
}

AGAS_HeroBase* AAIControllerBase::GetTargetHero()
{
	if (TargetHero)
	{
		return TargetHero;
	}
	else
	{
		return nullptr;
	}
}

AActor* AAIControllerBase::GetTargetActor()
{
	if (TargetHero)
	{
		return TargetHero;
	}
	else
	{
		return nullptr;
	}
}

float AAIControllerBase::GetTargetHeroDistance() const
{
	if (!IsValid(ControlledEnemy) || !IsValid(TargetHero))
	{
		return -1.0f;
	}

	FVector MyLocation = ControlledEnemy->GetActorLocation();
	FVector TargetLocation = TargetHero->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (UAC_Team* PawnTeamComponent = ControlledPawn->GetComponentByClass<UAC_Team>())
		{
			return PawnTeamComponent->GetTeamAttitudeTowards(Other);
		}
	}

	return ETeamAttitude::Neutral;
}

