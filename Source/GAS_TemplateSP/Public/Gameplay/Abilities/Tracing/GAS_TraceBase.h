// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "ScalableFloat.h"
#include "Gameplay/Components/AC_Team.h"
#include "GAS_TraceBase.generated.h"

UENUM(BlueprintType)
enum ETraceOrigin: uint8
{
	Avatar,
	Camera
};

UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInLineNew)
class GAS_TEMPLATESP_API UGAS_TraceBase : public UObject
{
	GENERATED_BODY()
	
protected:

	const AActor* OwnerActor = nullptr;

public:
	void CreateTraceFromTargetingDataWithTeamFilter(const UWorld* World, TArray<AActor*>& OutActors, AActor* Owner, ETeamAttitude::Type TeamAttidue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ETraceOrigin> TraceOrigin = ETraceOrigin::Avatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	bool bIgnoreSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceParams", meta = (ExposeOnSpawn = true))
	float TraceDistance = .0f;

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

	void MakeTeamFilter(TArray<AActor*>& OutActors, const AActor& Owner, ETeamAttitude::Type TeamAttidue);

	void MakeTrace(const UObject* Owner, const UWorld* World, const FVector& Location, const FRotator& Direction, TArray<AActor*>& OutActors);

	virtual void Initialize(const UObject* Owner, FRotator Direction);

	virtual void TraceLogic(const UWorld* World, const FVector& Location, const FRotator& Direction, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults);

	virtual FCollisionShape GetCollisionShape() const;


#if WITH_EDITOR
protected:
	virtual void DrawDebugShape(const UWorld* World, const FVector& Location) const;
#endif // WITH_EDITOR

};
