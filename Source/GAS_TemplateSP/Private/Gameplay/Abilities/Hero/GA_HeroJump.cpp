// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroJump.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"

bool UGA_HeroJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InAir))
	{
		return false;
	}

	return true;
}

void UGA_HeroJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Cache references
	OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!OwnerCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CharacterMovement = OwnerCharacter->GetCharacterMovement();
	if (!CharacterMovement)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Curve kontrolü
	if (!JumpCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_HeroJump: JumpCurve is not set! Using default jump."));
		CharacterMovement->DoJump(false);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Initialize jump
	JumpElapsedTime = 0.0f;
	OriginalGravityScale = CharacterMovement->GravityScale;
	CharacterMovement->GravityScale = JumpGravityScale;

	// Initial impulse
	FVector LaunchVelocity = FVector::UpVector * CalculateInitialVelocity();
	CharacterMovement->Velocity.Z = LaunchVelocity.Z + InitialImpulse;

	// Launch character
	CharacterMovement->SetMovementMode(MOVE_Falling);

	// Start curve-based jump timer
	GetWorld()->GetTimerManager().SetTimer(
		JumpTimerHandle,
		this,
		&UGA_HeroJump::TickJumpCurve,
		0.016f, // ~60fps tick rate
		true
	);
}

void UGA_HeroJump::TickJumpCurve()
{
	if (!CharacterMovement || !OwnerCharacter)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	JumpElapsedTime += 0.016f;

	// Jump süresi doldu mu?
	if (JumpElapsedTime >= JumpDuration)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Yere deðdi mi? (erken bitir)
	if (CharacterMovement->IsMovingOnGround())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Apply curve-based velocity
	ApplyJumpVelocity(0.016f);
}

void UGA_HeroJump::ApplyJumpVelocity(float DeltaTime)
{
	if (!JumpCurve || !CharacterMovement)
		return;

	// Normalize time (0-1)
	float NormalizedTime = FMath::Clamp(JumpElapsedTime / JumpDuration, 0.0f, 1.0f);

	// Curve'den deðer al (0-1 range)
	float CurveValue = JumpCurve->GetFloatValue(NormalizedTime);

	// Calculate target velocity
	float BaseVelocity = CalculateInitialVelocity();
	float TargetZVelocity = BaseVelocity * CurveValue;

	// Smooth blend (daha yumuþak geçiþ için)
	float CurrentZVelocity = CharacterMovement->Velocity.Z;
	float BlendedVelocity = FMath::FInterpTo(CurrentZVelocity, TargetZVelocity, DeltaTime, 10.0f);

	// Apply velocity
	CharacterMovement->Velocity.Z = BlendedVelocity;
}

float UGA_HeroJump::CalculateInitialVelocity() const
{
	// Physics formula: v = sqrt(2 * g * h)
	// Ama burada daha fazla kontrol için JumpHeight kullanýyoruz
	float Gravity = FMath::Abs(CharacterMovement->GetGravityZ()) * JumpGravityScale;
	return FMath::Sqrt(2.0f * Gravity * JumpHeight);
}

void UGA_HeroJump::RestoreGravity()
{
	if (CharacterMovement)
	{
		CharacterMovement->GravityScale = OriginalGravityScale;
	}
}

void UGA_HeroJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Cleanup
	if (JumpTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	}

	RestoreGravity();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}