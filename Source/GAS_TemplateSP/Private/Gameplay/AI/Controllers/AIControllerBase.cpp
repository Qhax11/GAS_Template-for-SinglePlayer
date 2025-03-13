// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Gameplay/Components/AC_Team.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

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

void AAIControllerBase::TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor) 
	{
		if (UBlackboardComponent* BlackboradComponent = GetBlackboardComponent()) 
		{
			BlackboradComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
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
