// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Abilities/Tasks/AT_MoveToActorForce_IgnoreZ.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "AbilitySystemComponent.h"

UAT_MoveToActorForce_IgnoreZ* UAT_MoveToActorForce_IgnoreZ::ApplyRootMotionMoveToActorForce_IgnoreZ(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	AActor* InTargetActor,
	FVector InTargetLocationOffset,
	ERootMotionMoveToActorTargetOffsetType InOffsetAlignment,
	float InDuration,
	UCurveFloat* InTargetLerpSpeedHorizontal,
	UCurveFloat* InTargetLerpSpeedVertical,
	bool bInSetNewMovementMode,
	EMovementMode InMovementMode,
	bool bInRestrictSpeedToExpected,
	UCurveVector* InPathOffsetCurve,
	UCurveFloat* InTimeMappingCurve,
	ERootMotionFinishVelocityMode InVelocityOnFinishMode,
	FVector InSetVelocityOnFinish,
	float InClampVelocityOnFinish,
	bool bInDisableDestinationReachedInterrupt,
	float InReachedDestinationDistance)
{
	UAT_MoveToActorForce_IgnoreZ* MyTask = NewAbilityTask<UAT_MoveToActorForce_IgnoreZ>(OwningAbility, TaskInstanceName);

	MyTask->ForceName = TaskInstanceName;
	MyTask->TargetActor = InTargetActor;
	MyTask->TargetComponent = nullptr;
	MyTask->TargetComponentRelativeLocation = FVector::ZeroVector;
	MyTask->TargetLocationOffset = InTargetLocationOffset;
	MyTask->OffsetAlignment = InOffsetAlignment;
	MyTask->Duration = FMath::Max(InDuration, KINDA_SMALL_NUMBER);
	MyTask->bDisableDestinationReachedInterrupt = bInDisableDestinationReachedInterrupt;
	MyTask->ReachedDestinationDistance = InReachedDestinationDistance;
	MyTask->TargetLerpSpeedHorizontalCurve = InTargetLerpSpeedHorizontal;
	MyTask->TargetLerpSpeedVerticalCurve = InTargetLerpSpeedVertical;
	MyTask->bSetNewMovementMode = bInSetNewMovementMode;
	MyTask->NewMovementMode = InMovementMode;
	MyTask->bRestrictSpeedToExpected = bInRestrictSpeedToExpected;
	MyTask->PathOffsetCurve = InPathOffsetCurve;
	MyTask->TimeMappingCurve = InTimeMappingCurve;
	MyTask->FinishVelocityMode = InVelocityOnFinishMode;
	MyTask->FinishSetVelocity = InSetVelocityOnFinish;
	MyTask->FinishClampVelocity = InClampVelocityOnFinish;
	MyTask->bHasCachedZ = false;
	MyTask->CachedTargetZ = 0.f;

	if (MyTask->GetAvatarActor() != nullptr)
	{
		MyTask->StartLocation = MyTask->GetAvatarActor()->GetActorLocation();
	}
	else
	{
		MyTask->StartLocation = InTargetActor ? InTargetActor->GetActorLocation() : FVector(0.f);
	}

	MyTask->SharedInitAndApply();

	return MyTask;
}

void UAT_MoveToActorForce_IgnoreZ::SharedInitAndApply()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && ASC->AbilityActorInfo->MovementComponent.IsValid())
	{
		MovementComponent = Cast<UCharacterMovementComponent>(ASC->AbilityActorInfo->MovementComponent.Get());
		StartTime = GetWorld()->GetTimeSeconds();
		EndTime = StartTime + Duration;

		if (MovementComponent.IsValid())
		{
			if (bSetNewMovementMode)
			{
				PreviousMovementMode = MovementComponent->MovementMode;
				PreviousCustomMode = MovementComponent->CustomMovementMode;
				MovementComponent->SetMovementMode(NewMovementMode);
			}

			// Ýlk target location'ý Z ignore ederek ayarla
			if (TargetActor)
			{
				TargetLocation = CalculateTargetOffset_IgnoreZ();
			}

			ForceName = ForceName.IsNone() ? FName("AbilityTaskApplyRootMotionMoveToActorForce_IgnoreZ") : ForceName;
			TSharedPtr<FRootMotionSource_MoveToDynamicForce> MoveToActorForce = MakeShared<FRootMotionSource_MoveToDynamicForce>();
			MoveToActorForce->InstanceName = ForceName;
			MoveToActorForce->AccumulateMode = ERootMotionAccumulateMode::Override;
			MoveToActorForce->Settings.SetFlag(ERootMotionSourceSettingsFlags::UseSensitiveLiftoffCheck);
			MoveToActorForce->Priority = 900;
			MoveToActorForce->InitialTargetLocation = TargetLocation;
			MoveToActorForce->TargetLocation = TargetLocation;
			MoveToActorForce->StartLocation = StartLocation;
			MoveToActorForce->Duration = FMath::Max(Duration, KINDA_SMALL_NUMBER);
			MoveToActorForce->bRestrictSpeedToExpected = bRestrictSpeedToExpected;
			MoveToActorForce->PathOffsetCurve = PathOffsetCurve;
			MoveToActorForce->TimeMappingCurve = TimeMappingCurve;
			MoveToActorForce->FinishVelocityParams.Mode = FinishVelocityMode;
			MoveToActorForce->FinishVelocityParams.SetVelocity = FinishSetVelocity;
			MoveToActorForce->FinishVelocityParams.ClampVelocity = FinishClampVelocity;
			RootMotionSourceID = MovementComponent->ApplyRootMotionSource(MoveToActorForce);
		}
	}
}

FVector UAT_MoveToActorForce_IgnoreZ::CalculateTargetOffset_IgnoreZ() const
{
	check(TargetActor != nullptr);

	const FVector TargetActorLocation = TargetComponent ?
		TargetComponent->GetComponentTransform().TransformPosition(TargetComponentRelativeLocation) :
		TargetActor->GetActorLocation();

	// Ýlk kez çaðrýlýyorsa, target'ýn Z'sini cache'le
	if (!bHasCachedZ)
	{
		const_cast<UAT_MoveToActorForce_IgnoreZ*>(this)->CachedTargetZ = TargetActorLocation.Z;
		const_cast<UAT_MoveToActorForce_IgnoreZ*>(this)->bHasCachedZ = true;
	}

	// Sadece X ve Y eksenini kullan, Z'yi sabit tut
	FVector CalculatedTargetLocation = FVector(TargetActorLocation.X, TargetActorLocation.Y, CachedTargetZ);

	if (OffsetAlignment == ERootMotionMoveToActorTargetOffsetType::AlignFromTargetToSource)
	{
		if (MovementComponent.IsValid())
		{
			FVector ToSource = MovementComponent->GetActorLocation() - FVector(TargetActorLocation.X, TargetActorLocation.Y, CachedTargetZ);
			ToSource.Z = 0.f;
			FVector OffsetRotated = ToSource.ToOrientationQuat().RotateVector(TargetLocationOffset);
			CalculatedTargetLocation.X += OffsetRotated.X;
			CalculatedTargetLocation.Y += OffsetRotated.Y;
			CalculatedTargetLocation.Z = CachedTargetZ + OffsetRotated.Z;
		}
	}
	else if (OffsetAlignment == ERootMotionMoveToActorTargetOffsetType::AlignToTargetForward)
	{
		FVector OffsetRotated = TargetActor->GetActorQuat().RotateVector(TargetLocationOffset);
		CalculatedTargetLocation.X += OffsetRotated.X;
		CalculatedTargetLocation.Y += OffsetRotated.Y;
		CalculatedTargetLocation.Z = CachedTargetZ + OffsetRotated.Z;
	}
	else if (OffsetAlignment == ERootMotionMoveToActorTargetOffsetType::AlignToWorldSpace)
	{
		CalculatedTargetLocation.X += TargetLocationOffset.X;
		CalculatedTargetLocation.Y += TargetLocationOffset.Y;
		CalculatedTargetLocation.Z = CachedTargetZ + TargetLocationOffset.Z;
	}

	return CalculatedTargetLocation;
}

void UAT_MoveToActorForce_IgnoreZ::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	// Parent'ýn TickTask'ýný deðil kendi implementasyonumuzu kullan
	UAbilityTask::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		const bool bTimedOut = HasTimedOut();

		// Target location'ý Z ignore ederek güncelle
		if (TargetActor && GetWorld())
		{
			const FVector PreviousTargetLocation = TargetLocation;
			FVector ExactTargetLocation = CalculateTargetOffset_IgnoreZ();

			const float CurrentTime = GetWorld()->GetTimeSeconds();
			const float CompletionPercent = (CurrentTime - StartTime) / Duration;

			const float TargetLerpSpeedHorizontal = TargetLerpSpeedHorizontalCurve ?
				TargetLerpSpeedHorizontalCurve->GetFloatValue(CompletionPercent) : 1000.f;

			const float MaxHorizontalChange = FMath::Max(0.f, TargetLerpSpeedHorizontal * DeltaTime);

			FVector ToExactLocation = ExactTargetLocation - PreviousTargetLocation;
			FVector TargetLocationDelta = ToExactLocation;

			// Z deðiþimini tamamen ignore et
			TargetLocationDelta.Z = 0.f;

			// Horizontal lerp
			if (FMath::Abs(ToExactLocation.SizeSquared2D()) > MaxHorizontalChange * MaxHorizontalChange)
			{
				FVector ToExactLocationHorizontal(ToExactLocation.X, ToExactLocation.Y, 0.f);
				ToExactLocationHorizontal.Normalize();
				ToExactLocationHorizontal *= MaxHorizontalChange;

				TargetLocationDelta.X = ToExactLocationHorizontal.X;
				TargetLocationDelta.Y = ToExactLocationHorizontal.Y;
			}

			TargetLocation += TargetLocationDelta;

			// Root motion target'ý güncelle
			if (MovementComponent.IsValid())
			{
				TSharedPtr<FRootMotionSource> RMS = MovementComponent->GetRootMotionSourceByID(RootMotionSourceID);
				if (RMS.IsValid())
				{
					if (RMS->GetScriptStruct() == FRootMotionSource_MoveToDynamicForce::StaticStruct())
					{
						FRootMotionSource_MoveToDynamicForce* MoveToActorForce = static_cast<FRootMotionSource_MoveToDynamicForce*>(RMS.Get());
						if (MoveToActorForce)
						{
							MoveToActorForce->SetTargetLocation(TargetLocation);
						}
					}
				}
			}
		}

		// Destination check - sadece XY düzleminde kontrol et
		const float ReachedDestinationDistanceSqr = ReachedDestinationDistance * ReachedDestinationDistance;
		const FVector MyLocation2D = FVector(MyActor->GetActorLocation().X, MyActor->GetActorLocation().Y, 0.f);
		const FVector TargetLocation2D = FVector(TargetLocation.X, TargetLocation.Y, 0.f);
		const bool bReachedDestination = FVector::DistSquared(TargetLocation2D, MyLocation2D) < ReachedDestinationDistanceSqr;

		if (bTimedOut || (bReachedDestination && !bDisableDestinationReachedInterrupt))
		{
			bIsFinished = true;
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnFinished.Broadcast(bReachedDestination, bTimedOut, TargetLocation);
				}
				EndTask();
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}