// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;          
	PrimaryActorTick.bStartWithTickEnabled = false; 

	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Weapon mesh
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// Trace points
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(WeaponMesh);

	TraceMid = CreateDefaultSubobject<USceneComponent>(TEXT("TraceMid"));
	TraceMid->SetupAttachment(WeaponMesh);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(WeaponMesh);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	AGAS_CharacterBase* OwnerCharacter = Cast<AGAS_CharacterBase>(GetAttachParentActor());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is null in: %s"), *GetClass()->GetName());
		return;
	}

	UAC_TagDelegates* OwnerTagDelegateComp = OwnerCharacter->GetTagDelegatesComponent();
	if (!OwnerTagDelegateComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerTagDelegateComp is null in: %s"), *GetClass()->GetName());
		return;
	}

	OwnerTagDelegateComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Hit, EListenMode::OnAdded).BindDynamic(this, &AWeaponBase::OnPhaseActivePostHitTagAdded);
	OwnerTagDelegateComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Hit, EListenMode::OnRemoved).BindDynamic(this, &AWeaponBase::OnPhaseActivePostHitTagRemoved);

	SetActorTickEnabled(false);      // explicitly off first
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsTracking)
		return;

	const FVector Mid = GetTraceMid();

	// Debug points
	//DrawDebugPoint(GetWorld(), Mid, 8.f, FColor::Green, false, 0.05f);
	//DrawDebugPoint(GetWorld(), PreviousMid, 8.f, FColor::Red, false, 0.05f);

	// First valid frame
	if (PreviousMid.IsZero())
	{
		PreviousMid = Mid;
		SwingDirection = FVector::ZeroVector;
		return;
	}

	// Calculate movement direction
	FVector Delta = Mid - PreviousMid;
	SwingDirection = Delta.GetSafeNormal();

	PreviousMid = Mid;
}

void AWeaponBase::OnPhaseActivePostHitTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EnableTracking();
}

void AWeaponBase::OnPhaseActivePostHitTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	DisableTracking();
}

void AWeaponBase::EnableTracking()
{
	if (bIsTracking)
		return;

	bIsTracking = true;

	PreviousMid = FVector::ZeroVector;
	SwingDirection = FVector::ZeroVector;

	SetActorTickEnabled(true);

	UE_LOG(LogTemp, Warning, TEXT("Tracking enabled"));
}

void AWeaponBase::DisableTracking()
{
	if (!bIsTracking)
		return;

	bIsTracking = false;
	SetActorTickEnabled(false);
}

FVector AWeaponBase::GetTraceStart() const
{
	return TraceStart ? TraceStart->GetComponentLocation() : FVector::ZeroVector;
}

FVector AWeaponBase::GetTraceMid() const
{
	return TraceMid ? TraceMid->GetComponentLocation() : FVector::ZeroVector;
}

FVector AWeaponBase::GetTraceEnd() const
{
	return TraceEnd ? TraceEnd->GetComponentLocation() : FVector::ZeroVector;
}

FRotator AWeaponBase::GetTraceEndRotation() const
{
	return TraceEnd ? TraceEnd->GetComponentRotation() : FRotator::ZeroRotator;
}

