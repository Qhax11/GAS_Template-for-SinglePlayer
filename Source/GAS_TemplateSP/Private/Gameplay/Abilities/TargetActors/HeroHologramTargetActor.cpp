// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/HeroHologramTargetActor.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"


void AHeroHologramTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	HeroBase = Cast<AGAS_HeroBase>(OwningAbility->GetAvatarActorFromActorInfo());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
		return;
	}

	EyeOfViewComponent = HeroBase->GetEyeOfViewComponent();
	if(!EyeOfViewComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EyeOfViewComponent is null in: %s"), *GetName());
		return;
	}

	TargetLockSystemComponent = HeroBase->GetTargetLockSystemComponent();
	if (!TargetLockSystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLockSystemComponent is null in: %s"), *GetName());
		return;
	}

	if (TargetLockSystemComponent->CurrentTarget) 
	{
		CurrentTarget = TargetLockSystemComponent->CurrentTarget;
	}
	TargetLockSystemComponent->OnTargetChanged.AddDynamic(this, &AHeroHologramTargetActor::OnTargetChaned);
	TargetLockSystemComponent->OnEndTargetLock.AddDynamic(this, &AHeroHologramTargetActor::OnEndTargetLock);
}

void AHeroHologramTargetActor::ConfirmTargetingAndContinue()
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;

	// Konum bilgisi için bir TargetData_LocationInfo oluþtur
	FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
	LocationData->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;

	// Konumu TargetData'ya ekle
	FTransform TargetTransform;
	TargetTransform.SetLocation(GetActorLocation());
	LocationData->TargetLocation.LiteralTransform = TargetTransform;

	// TargetDataHandle'a veriyi ekle
	TargetDataHandle.Add(LocationData);

	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}

void AHeroHologramTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AHeroHologramTargetActor::OnTargetChaned(AActor* NewTarget)
{
	BP_OnTargetChaned(NewTarget);
}

void AHeroHologramTargetActor::OnEndTargetLock()
{
	BP_OnEndTargetLock();
}
