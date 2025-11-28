// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GAS_UtilityLibrary.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGAS_UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "GAS_UtilityLibrary")
	static AActor* FindNearestActor(AActor* ReferanceActor, TArray<AActor*> ActorArray);

	UFUNCTION(BlueprintCallable, Category = "GAS_UtilityLibrary")
	static void FilterOutDeadActors(TArray<AActor*>& Actors);

};
