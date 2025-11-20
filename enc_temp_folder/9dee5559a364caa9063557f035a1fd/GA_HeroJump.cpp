// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroJump.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"

UGA_HeroJump::UGA_HeroJump()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback);
}

void UGA_HeroJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	HeroBase = Cast<AGAS_HeroBase>(CharacterBase);
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s)"), *GetName());
		return;
	}

	HeroControl = HeroBase->GetHeroControlComponent();
	if (!HeroControl)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroControl is null in: %s)"), *GetName());
		return;
	}

	FGameplayTagContainer CancelAbilityTags;
	CancelAbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack);
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelAbilityTags);

	HeroMovement = HeroBase->GetCharacterMovement();

	float JumpStartAnimLenght = AnimMontage->GetPlayLength();
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, [this]()
		{
			JumpLogic();
		}, JumpStartAnimLenght - 0.2f, false);
}

void UGA_HeroJump::JumpLogic() 
{
	// Kamera yönünü al (Controller'ýn rotation'ý)
	APlayerController* PC = Cast<APlayerController>(HeroBase->GetController());
	if (!PC)
	{
		return;
	}

	FRotator ControlRotation = PC->GetControlRotation();

	// Sadece yaw'ý kullan (pitch ve roll'u sýfýrla, yoksa yukarý/aþaðý bakarken garip olur)
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	// Kamera bazlý ileri ve sað vektörleri
	FVector CameraForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector CameraRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Input'u kamera yönüne göre hesapla
	FVector2D Input = HeroControl->LastMovementInput;
	FVector InputDirection = (CameraForward * Input.Y + CameraRight * Input.X).GetSafeNormal();

	float JumpVelocityZ = FMath::Sqrt(2.0f * FMath::Abs(HeroMovement->GetGravityZ()) * JumpHeight);

	FVector NewVelocity;
	if (InputDirection.IsNearlyZero())
	{
		NewVelocity = FVector(0, 0, JumpVelocityZ);
	}
	else
	{
		NewVelocity = InputDirection * GroundJumpForwardStrength + FVector(0, 0, JumpVelocityZ);
	}

	HeroBase->LaunchCharacter(NewVelocity, false, true);

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_HeroJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (JumpTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}