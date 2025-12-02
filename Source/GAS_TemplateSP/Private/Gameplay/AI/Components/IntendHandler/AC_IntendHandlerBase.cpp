// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "Gameplay/Animation/AN_SendGameplayEvent.h"
#include <Kismet/GameplayStatics.h>

UAC_IntendHandlerBase::UAC_IntendHandlerBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_IntendHandlerBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AAIControllerBase>(GetOwner());
	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s, can not initialize"), *GetName());
		return;
	}

	OwnerStateManager = OwnerController->GetEnemyStateManagerComponent();
	if (!OwnerStateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerStateManager is null in: %s, can not initialize"), *GetName());
		return;
	}

	OwnerBehaviorDecisionComp = OwnerController->GetBehaviorDecisionComponent();
	if (!OwnerBehaviorDecisionComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerBehaviorDecisionComp is null in: %s, can not initialize"), *GetName());
		return;
	}

	ControlledEnemy = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
	if (!ControlledEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlledEnemy is null in: %s, can not initialize"), *GetName());
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TargetHero = Cast<AGAS_HeroBase>(PlayerPawn);
	if (!TargetHero) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetHero is null in: %s, can not initialize"), *GetName());
		return;
	}

	US_AICrowdEventManager* AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>();
	if (!AICrowdEventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager is null in: %s, can not initialize"), *GetName());
		return;
	}

	RegisterTags(TargetHero);
	OwnerController->OnTargetDetected.AddDynamic(this, &UAC_IntendHandlerBase::OnTargetDetected);
	AICrowdEventManager->OnRequestEnemyBackupReaction.AddDynamic(this, &UAC_IntendHandlerBase::OnRequestEnemyBackupReaction);
}

void UAC_IntendHandlerBase::OnTargetDetected(AActor* DetectedTarget)
{
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_TargetDetected);
	TargetHero->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(this, &UAC_IntendHandlerBase::OnTargetAbilityActivated);
}

void UAC_IntendHandlerBase::OnRequestEnemyBackupReaction()
{
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_BackupReaction);
}

bool UAC_IntendHandlerBase::RegisterTags(AGAS_CharacterBase* TargetCharacter)
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

	ControlledCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, EListenMode::OnAdded).BindDynamic(this, &UAC_IntendHandlerBase::OnVulnerableTagAdded);
	return true;
}

void UAC_IntendHandlerBase::OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_VulnerableTagAdded);
}

void UAC_IntendHandlerBase::OnTargetAbilityActivated(UGameplayAbility* Ability)
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

	float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility, CombinedTags);
	FComingAttackPayload Payload(MeleeAttackAbility, AttackTime, CombinedTags);
	SendEventToDefense(Payload);
}

float UAC_IntendHandlerBase::GetAttackNotifyTriggerTime(UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags)
{
	if (!Ability || !Ability->AnimMontage)
	{
		return -1.0f;
	}

	const UAnimMontage* Montage = Ability->AnimMontage;

	// 1. TraceStart notify'inin süresini bul
	float NotifyTime = -1.0f;

	for (const FAnimNotifyEvent& Notify : Montage->Notifies)
	{
		if (const UAN_SendGameplayEvent* TagNotify = Cast<UAN_SendGameplayEvent>(Notify.Notify))
		{
			if (TagNotify->EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_TraceStart)
			{
				NotifyTime = Notify.GetTriggerTime();
				break;
			}
		}
	}

	if (NotifyTime < 0.f)
	{
		return -1.0f;
	}

	// 2. Eðer ShadowLinked tag'i varsa Section2 offsetini çýkar
	if (AbilityTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeCombo_ShadowLinked))
	{
		const FName SectionName = FName("Section2");
		const int32 SectionIndex = Montage->GetSectionIndex(SectionName);

		if (Montage->CompositeSections.IsValidIndex(SectionIndex))
		{
			const float SectionStartTime = Montage->CompositeSections[SectionIndex].GetTime();
			return FMath::Max(NotifyTime - SectionStartTime, 0.0f);
		}
	}

	// 3. Normal durumda NotifyTime döner
	return NotifyTime;
}

void UAC_IntendHandlerBase::SendEventToDefense(FComingAttackPayload EventPayload)
{
	UComingAttackReactionData* BestReactionData = OwnerBehaviorDecisionComp->GetBestComingAttackReaction(EventPayload);
	if (!BestReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("BestReaction is null in: %s"), *GetName());
		return;
	}

	HandleReactionTiming(BestReactionData, EventPayload);
}

void UAC_IntendHandlerBase::HandleReactionTiming(UComingAttackReactionData* Reaction, FComingAttackPayload Payload)
{
	if (Reaction->ReactionType == EComingAttackReaction::TakeDamage)
	{
		TriggerIncomingAttackReaction(Reaction, Payload);
		return;
	}

	const float PreferredDelay = Payload.ComingAttackHitTime - Reaction->PreferredTriggerTimeBeforeHit;

	if (PreferredDelay <= 0.f)
	{
		TriggerIncomingAttackReaction(Reaction, Payload);
	}
	else
	{
		FTimerHandle ReactionDelayTimer;
		GetWorld()->GetTimerManager().SetTimer(ReactionDelayTimer, FTimerDelegate::CreateUObject(
			this, &UAC_IntendHandlerBase::TriggerIncomingAttackReaction, Reaction, Payload), PreferredDelay, false);

		UE_LOG(LogTemp, Warning, TEXT("IncomingAttack Reaction delayed by %.2f seconds."), PreferredDelay);
	}
}

void UAC_IntendHandlerBase::TriggerIncomingAttackReaction(UComingAttackReactionData* Reaction, FComingAttackPayload Payload)
{
	OwnerStateManager->ComingAttackPayload = Payload;
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_InComingAttack);
}

