// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Gameplay/Components/AC_Team.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Animation/AN_SendTag.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"


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

void AAIControllerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
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
	if (!ControlledEnemy || !TargetCharacter)
	{
		return false;
	}

	UAC_TagDelegates* ControlledCharacterTagDelegatesComp = ControlledEnemy->GetTagDelegatesComponent();
	if (!ControlledCharacterTagDelegatesComp) 
	{
		return false;
	}

	UAC_TagDelegates* TargetCharacterTagDelegatesComp = TargetCharacter->GetTagDelegatesComponent();
	if (!TargetCharacterTagDelegatesComp)
	{
		return false;
	}

	TargetCharacter->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(this, &AAIControllerBase::OnTargetAbilityActivated);
	ControlledCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Vulnerable, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnVulnerableTagAdded);
	ControlledCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBase::OnTakeDamageTagAdded);
	return false;
}

void AAIControllerBase::OnTargetAbilityActivated(UGameplayAbility* Ability)
{
	if (!Ability)
	{
		return;
	}

	UGA_MeleeAttackBase* MeleeAttackAbility = Cast<UGA_MeleeAttackBase>(Ability);
	if (!MeleeAttackAbility)
	{
		return;
	}

	FGameplayTagContainer CombinedTags;

	// Add static tags
	CombinedTags.AppendTags(Ability->GetAssetTags()); 

	// Add dynamic tags from current spec
	if (const FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec())
	{
		CombinedTags.AppendTags(Spec->DynamicAbilityTags);
	}

	FTimerHandle SendEventTimer;
	FTimerDelegate TimerDelegate;
	FComingAttackPayload Payload(Ability, CombinedTags);
	TimerDelegate.BindLambda([this, Payload]()
		{
			this->SendEventToDefense(Payload);
		});

	float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility);
	if (AttackTime < 0) 
	{
		return;
	}

	float ReactionDelay = AttackTime - 0.5;
	if (ReactionDelay > 0) 
	{
		GetWorld()->GetTimerManager().SetTimer(SendEventTimer, TimerDelegate, ReactionDelay, false);
	}
	else
	{
		SendEventToDefense(Payload);
	}
}

float AAIControllerBase::GetAttackNotifyTriggerTime(UGA_MeleeAttackBase* Ability) const
{
	if (!Ability || !Ability->AnimMontage)
	{
		return -1.0f;
	}

	for (const FAnimNotifyEvent& Notify : Ability->AnimMontage->Notifies)
	{
		if (const UAN_SendTag* TagNotify = Cast<UAN_SendTag>(Notify.Notify))
		{
			if (TagNotify->NotifyTag == GAS_Tags::TAG_Gameplay_AttackEvent_TraceStart)
			{
				return Notify.GetTriggerTime();
			}
		}
	}

	return -1.0f;
}

void AAIControllerBase::SendEventToDefense(FComingAttackPayload EventPayload)
{
	ControlledEnemy->GetEnemyStateManagerComponent()->ComingAttackPayload = EventPayload;
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_PlayerStartedAttack, FConstStructView::Make(EventPayload));
}

void AAIControllerBase::OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_State_Vulnerable);
}

void AAIControllerBase::OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_TakeDamage);
}
