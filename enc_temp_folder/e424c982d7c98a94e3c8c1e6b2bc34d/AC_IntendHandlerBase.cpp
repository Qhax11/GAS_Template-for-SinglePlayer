// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"
#include "Gameplay/Animation/ANS_AttackTrace.h"
#include "Gameplay/AI/States/InComingAttackState.h"
#include <Kismet/GameplayStatics.h>

UAC_IntendHandlerBase::UAC_IntendHandlerBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_IntendHandlerBase::BeginPlay()
{
	Super::BeginPlay();

	checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());
	checkf(OwnerEnemyBase, TEXT("OwnerEnemyBase is null in %s"), *GetClass()->GetName());
	checkf(OwnerEnemyASC, TEXT("OwnerEnemyASC is null in %s"), *GetClass()->GetName());

	OwnerStateManager = OwnerController->GetEnemyStateManagerComponent();
	checkf(OwnerStateManager, TEXT("OwnerStateManager is null in %s"), *GetClass()->GetName());

	OwnerBehaviorDecisionComp = OwnerController->GetBehaviorDecisionComponent();
	checkf(OwnerBehaviorDecisionComp, TEXT("OwnerBehaviorDecisionComp is null in %s"), *GetClass()->GetName());

	US_AICrowdEventManager* AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>();
	checkf(AICrowdEventManager, TEXT("AICrowdEventManager is null in %s"), *GetClass()->GetName());

	DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>();
	checkf(DamageSubsystem, TEXT("DamageSubsystem is null in %s"), *GetClass()->GetName());

	RegisterTags();
	DamageSubsystem->OnDamageDealt.AddDynamic(this, &UAC_IntendHandlerBase::OnDamageDealt);
	OwnerController->OnTargetDetected.AddDynamic(this, &UAC_IntendHandlerBase::OnTargetDetected);
	AICrowdEventManager->OnRequestEnemyBackupReaction.AddDynamic(this, &UAC_IntendHandlerBase::OnRequestEnemyBackupReaction);
}

void UAC_IntendHandlerBase::OnTargetDetected(AActor* DetectedTarget)
{
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_TargetDetected);
	HeroBase->GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddUObject(this, &UAC_IntendHandlerBase::OnTargetAbilityActivated);
}

void UAC_IntendHandlerBase::OnRequestEnemyBackupReaction()
{
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_BackupReaction);
}

void UAC_IntendHandlerBase::RegisterTags()
{
	UAC_TagDelegates* ControlledCharacterTagDelegatesComp = OwnerEnemyBase->GetTagDelegatesComponent();
	checkf(ControlledCharacterTagDelegatesComp, TEXT("ControlledCharacterTagDelegatesComp is null in %s"), *GetClass()->GetName());

	ControlledCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, EListenMode::OnAdded).BindDynamic(this, &UAC_IntendHandlerBase::OnVulnerableTagAdded);
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

	FGameplayTag AttackTypeTag = MeleeAttackAbility->GetAttackTypeTagFromAbilityTags();
	FGameplayTag AttackDirectionTag = MeleeAttackAbility->GetAttackDirectionTagFromAbilityTags();
	float AttackTime = GetAttackNotifyTriggerTime(MeleeAttackAbility, CombinedTags);

	FComingAttackPayload Payload(MeleeAttackAbility, AttackTime, CombinedTags, AttackTypeTag, AttackDirectionTag, OwnerEnemyASC);
	SendEventToDefense(Payload);
}

float UAC_IntendHandlerBase::GetAttackNotifyTriggerTime(UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags)
{
	if (!Ability || !Ability->AnimMontage)
	{
		return -1.0f;
	}

	const UAnimMontage* Montage = Ability->AnimMontage;
	float NotifyStartTime = -1.0f;

	for (const FAnimNotifyEvent& NotifyEvent : Montage->Notifies)
	{
		if (NotifyEvent.NotifyStateClass && NotifyEvent.NotifyStateClass->IsA<UANS_AttackTrace>())
		{
			const UANS_AttackTrace* StateNotify = Cast<UANS_AttackTrace>(NotifyEvent.NotifyStateClass);
			if (StateNotify->EventTagStart == GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Start)
			{
				NotifyStartTime = NotifyEvent.GetTime(); // NotifyBegin zamaný
				break;
			}
		}
	}

	if (NotifyStartTime < 0.f)
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
			return FMath::Max(NotifyStartTime - SectionStartTime, 0.0f);
		}
	}

	// 3. Normal durumda NotifyTime döner
	return NotifyStartTime;
}

void UAC_IntendHandlerBase::SendEventToDefense(FComingAttackPayload& ComingAttackPayload)
{
	UComingAttackReactionData* SelectedReaction = OwnerBehaviorDecisionComp->GetBestComingAttackReaction(ComingAttackPayload);
	if (!SelectedReaction)
	{
		UE_LOG(LogTemp, Warning, TEXT("BestReaction is null in: %s"), *GetName());
		return;
	}

	// TakeDamage event will triggered from UAC_IntendHandlerBase::OnDamageDealt;
	if (SelectedReaction->ReactionType == EComingAttackReaction::TakeDamage)
	{
		return;
	}

	const float PreferredDelay = ComingAttackPayload.ComingAttackHitTime - SelectedReaction->PreferredTriggerTimeBeforeHit;
	if (PreferredDelay <= 0.f)
	{
		TriggerIncomingAttackReaction(SelectedReaction, ComingAttackPayload);
	}
	else
	{
		FTimerHandle ReactionDelayTimer;
		GetWorld()->GetTimerManager().SetTimer(ReactionDelayTimer, FTimerDelegate::CreateUObject(
			this, &UAC_IntendHandlerBase::TriggerIncomingAttackReaction, SelectedReaction, ComingAttackPayload), PreferredDelay, false);

		UE_LOG(LogTemp, Warning, TEXT("IncomingAttack Reaction delayed by %.2f seconds."), PreferredDelay);
	}
}

void UAC_IntendHandlerBase::TriggerIncomingAttackReaction(UComingAttackReactionData* Reaction, FComingAttackPayload Payload)
{
	TSharedPtr<FIncomingAttackStatePayload> IncomingAttackStatePayload = MakeShared<FIncomingAttackStatePayload>(Payload, Reaction);
	OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_InComingAttack, IncomingAttackStatePayload);
}

void UAC_IntendHandlerBase::OnDamageDealt(const FDamageData& DamageData)
{
	if (DamageData.bParrySucces) 
	{
		return;
	}

	// If the damage is from Hero to Enemy, trigger the event
	if (DamageData.ExecCalculationParameters.SourceActor == HeroBase && DamageData.ExecCalculationParameters.TargetActor == OwnerEnemyBase)
	{
		TSharedPtr<FTakeHitStatePayload> TakeHitStatePayload = MakeShared<FTakeHitStatePayload>(DamageData);
		OwnerStateManager->HandleIncomingEvent(GAS_Tags::TAG_AI_StateEvent_TakeHit, TakeHitStatePayload);
	}
}

