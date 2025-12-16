// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroAirKick.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

UGA_HeroAirKick::UGA_HeroAirKick()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_TopToBottom);
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Unparryable);
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_CanInterruptUnstoppable);

	ActivationRequiredTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_AirKick);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnparryableAttack);
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnDodgebleAttack);

	MontageEndPolicy = EMontageEndPolicy::Never;
}

void UGA_HeroAirKick::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CharacterBase)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CharacterBase->LandedDelegate.AddDynamic(this, &UGA_HeroAirKick::OnCharacterLanded);
}

void UGA_HeroAirKick::GetTracePoints(FVector& OutStart, FVector& OutEnd, FRotator& OutRot)
{
	const USkeletalMeshComponent* Mesh = CharacterBase->GetMesh();
	OutStart = Mesh->GetBoneLocation(TEXT("calf_l"));      // ya da "calf_r"
	OutEnd = Mesh->GetBoneLocation(TEXT("foot_l"));      // ya da "foot_r"

	// Foot bone normalde ayaðýn forward yönünü doðru verir
	OutRot = Mesh->GetBoneQuaternion(TEXT("foot_l")).Rotator();
}

void UGA_HeroAirKick::AttackLogic(const TArray<FHitResult>& OutHitResults)
{
	Super::AttackLogic(OutHitResults);

	if (!OutHitResults.IsValidIndex(0))
	{
		return;
	}

	const FHitResult& Hit = OutHitResults[0];
	AActor* TargetActor = Hit.GetActor();
	if (!TargetActor)
	{
		return;
	}

	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (!TargetCharacter)
	{
		return;
	}

	FVector KnockbackDir = Hit.ImpactNormal;
	KnockbackDir = KnockbackDir.GetSafeNormal();
	KnockbackDir.Z = ZStrength;

	CharacterBase->LaunchCharacter(KnockbackDir * LaunchStrength, true, true);

	UAnimInstance* AnimInstance = CharacterBase->GetMesh()->GetAnimInstance();
	if (AnimInstance && ReverseJump)
	{
		AnimInstance->Montage_Play(ReverseJump, 1.0f);
	}

	if (bEnableDebug)
	{
		DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + KnockbackDir * LaunchStrength * 10.1f, FColor::Red, false, 2.f, 0, 2.f);
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.f, FColor::Yellow, false, 2.f);
	}
}

void UGA_HeroAirKick::OnCharacterLanded(const FHitResult& Hit)
{
	if (CharacterBase)
	{
		CharacterBase->LandedDelegate.RemoveDynamic(this, &UGA_HeroAirKick::OnCharacterLanded);
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
