// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

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
