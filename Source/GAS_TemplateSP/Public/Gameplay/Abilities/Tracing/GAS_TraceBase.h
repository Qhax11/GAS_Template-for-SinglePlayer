// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "ScalableFloat.h"
#include "Gameplay/Components/AC_Team.h"
#include "GAS_TraceBase.generated.h"

UENUM(BlueprintType)
enum ETraceStartLocation: uint8
{
	Avatar,
	Camera,
	CustomLocation
};

UENUM(BlueprintType)
enum ETraceDirectionType : uint8
{
	ForwardVector,
	CustomDirection
};

#define ECC_DEAD ECC_GameTraceChannel1
#define ECC_DAMAGE ECC_GameTraceChannel2

UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInLineNew)
class GAS_TEMPLATESP_API UGAS_TraceBase : public UObject
{
	GENERATED_BODY()
	
protected:

	const AActor* OwnerActor = nullptr;

public:
	void CreateTraceWithTeamFilter(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, TArray<AActor*>& OutActors);

	void CreateTraceWithTeamFilterWithLocation(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, FVector& Location, TArray<AActor*>& OutActors);

	void CreateTraceWithTeamFilterWithDirection(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, FRotator& Direction, TArray<AActor*>& OutActors);

	void CreateTraceWithTeamFilterWithLocationAndDirection(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, FVector& Location, FRotator& Direction, TArray<AActor*>& OutActors);

	void CreateTraceWithTeamFilterWithLocationAndDirection(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, FVector& Location, FRotator& Direction, TArray<FHitResult>& HitResults);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ETraceStartLocation> TraceStartLocation = ETraceStartLocation::Avatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ETraceDirectionType> TraceDirectionType = ETraceDirectionType::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	bool bIgnoreSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_DAMAGE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	float TraceDistance = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	bool bUseStartLocationForwardOffset = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true), meta = (EditCondition = "bUseStartLocationForwardOffset"))
	float StartLocationForwardOffset = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams")
	bool bOverrideTraceDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true), meta = (EditCondition = "bOverrideTraceDirection"))
	FRotator TraceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ECollisionResponse> CollisionResponse = ECollisionResponse::ECR_Overlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams")
	bool bSingleTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawParams")
	bool bDrawEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawParams", meta = (EditCondition = "bDrawEnable"))
	float DebugShapeDrawDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawParams", meta = (EditCondition = "bDrawEnable"))
	FColor DrawColor = FColor::White;

protected:

	void GetTraceStartLocationAndDirection(AActor* Owner, FVector& OutStartLocation, FRotator& OutDirection);

	void MakeTeamFilter(TArray<AActor*>& OutActors, const AActor& Owner, ETeamAttitude::Type TeamAttidue);

	void MakeTeamFilter(TArray<FHitResult>& OutHitResults, const AActor& Owner, ETeamAttitude::Type TeamAttidue);

	void MakeTrace(const UObject* Owner, const UWorld* World, const FVector& Location, const FRotator& Direction, TArray<AActor*>& OutActors);

	void MakeTrace(const UObject* Owner, const UWorld* World, const FVector& Location, const FRotator& Direction, TArray<FHitResult>& OutHitResults);

	virtual void Initialize(const UObject* Owner, FRotator Direction);

	virtual void TraceLogic(const UWorld* World, const FVector& Location, const FRotator& Direction, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults);

	virtual FCollisionShape GetCollisionShape() const;


#if WITH_EDITOR
protected:
	virtual void DrawDebugShape(const UWorld* World, const FVector& Location) const;
#endif // WITH_EDITOR

};
