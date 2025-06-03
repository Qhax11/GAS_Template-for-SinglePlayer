// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerCrowd.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerCrowd.h"


AAIControllerCrowd::AAIControllerCrowd(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) 
{
    IntendHandlerCrowdComponent = CreateDefaultSubobject<UAC_IntendHandlerCrowd>(TEXT("IntendHandlerCrowdComponent"));
}

