// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossDash.h"

FVector UGA_BossDash::CalculateDestination()
{
	return 	GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * DistanceMultiplier;
}
