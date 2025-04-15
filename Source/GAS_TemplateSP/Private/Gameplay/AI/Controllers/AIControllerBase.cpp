// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Gameplay/Components/AC_Team.h"
#include "BehaviorTree/BlackboardComponent.h"
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

	ControlledCharacter = Cast<AGAS_CharacterBase>(GetPawn());
	if (!ControlledCharacter) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledCharacter is null in: %s, Controller can not initialize"), *GetName());
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

		Target = Actor;
		OnTargetDetected.Broadcast(Target);
		StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_DetectedPlayer);
		RegisterTags(TargetCharacter);
		bHasTargetBeenDetected = true;
	}
}

AActor* AAIControllerBase::GetTarget()
{
	if (Target) 
	{
		return Target;
	}
	else
	{
		return nullptr;
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

bool AAIControllerBase::RegisterTags(AGAS_CharacterBase* TargetCharacter)
{
	if (!ControlledCharacter || !TargetCharacter)
	{
		return false;
	}

	if (UAC_TagDelegates* ControlledCharacterTagDelegatesComp = ControlledCharacter->GetTagDelegatesComponent())
	{
		ControlledCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Vulnerable, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnVulnerableTagAdded);
		return true;
	}

	if (UAC_TagDelegates* TargetCharacterTagDelegatesComp = TargetCharacter->GetTagDelegatesComponent())
	{
		TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo1, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnPlayerStartedAttackTagAdded);
		TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo2, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnPlayerStartedAttackTagAdded);
		TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo3, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnPlayerStartedAttackTagAdded);
	}

	return false;
}

void AAIControllerBase::OnPlayerStartedAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_PlayerStartedAttack);
}

void AAIControllerBase::OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_State_Vulnerable);
}
