// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroDash.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Tags/GAS_Tags.h"


void UGA_HeroDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!HeroBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Targeting_Hero_TargetLocked))
	{
		if (UAC_HeroControl* GetHeroControlComponent = HeroBase->GetHeroControlComponent())
		{
			//GetHeroControlComponent->las
		}
	}
	
	UAbilityTask_ApplyRootMotionMoveToForce* RootMotionTask =
		UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
			this,
			TEXT("None"),
			GetAvatarActorFromActorInfo()->GetActorRotation().Vector(),
			Duration,
			bSetNewMovementMode,
			NewMovementMode,
			bRestrictSpeedToExpected,
			DashCurve,
			FinishVelocityMode,
			FinishSetVelocity,
			FinishClampVelocity);

	RootMotionTask->ReadyForActivation();
	
}

void UGA_HeroDash::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);
	
	
}

