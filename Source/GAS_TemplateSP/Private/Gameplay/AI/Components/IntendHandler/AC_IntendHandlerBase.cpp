// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"

UAC_IntendHandlerBase::UAC_IntendHandlerBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_IntendHandlerBase::BeginPlay()
{
	Super::BeginPlay();
}

