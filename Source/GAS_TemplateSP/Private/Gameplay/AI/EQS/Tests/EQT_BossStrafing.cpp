// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_BossStrafing.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Gameplay/AI/Controllers/AIControllerBoss.h"


UEQT_BossStrafing::UEQT_BossStrafing()
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
    SetWorkOnFloatValues(true);
}

void UEQT_BossStrafing::RunTest(FEnvQueryInstance& QueryInstance) const
{
    AAIControllerBoss* AIBossController = Cast<AAIControllerBoss>(QueryInstance.Owner.Get());
    if (!AIBossController)
    {
        return;
    }

    APawn* Boss = AIBossController->GetPawn();
    if (!Boss)
    {
        return;
    }

    FVector BossLocation = Boss->GetActorLocation();
    FVector BossForward = Boss->GetActorForwardVector();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        FVector QueryPoint = GetItemLocation(QueryInstance, It.GetIndex());

        FVector BossLocationToPointDirection = QueryPoint - BossLocation;

        if (BossLocationToPointDirection.IsNearlyZero())
        {
            continue;
        }

        FVector BossLocationToPointDirectionNormalized = BossLocationToPointDirection.GetSafeNormal();

        float CrossZ = FVector::CrossProduct(BossForward, BossLocationToPointDirectionNormalized).Z;
        bool bIsRight = (CrossZ > 0.0f);  // Pozitifse sağ, negatifse sol

        // ✅ Enum'a göre yön filtresi (sadece doğru yönü skorla)
        if ((StrafeDirection == EStrafeDirection::Left && bIsRight) ||
            (StrafeDirection == EStrafeDirection::Right && !bIsRight))
        {
            It.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, 0.0f, 0.0f, 1.0f);
            continue; // Yanlış yön → atla
        }

        // 2️⃣ **Açıya Göre Skorlama (Dot Product ile)**
        float AngleScore = FVector::DotProduct(BossForward, BossLocationToPointDirectionNormalized);

        // Ön taraf kötü, sağ/sol uç noktalar iyi (Strafing için)
        float StrafingScore = 1.0f - FMath::Abs(AngleScore);

        // 3️⃣ **Ön Tarafa Gitmeyi Önleme**
        if (AngleScore > 0.8f)  // Eğer nokta boss'un tam önündeyse cezalandır
        {
            StrafingScore *= 0.5f;  // Skoru düşür
        }

        It.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, StrafingScore, 0.0f, 1.0f);
    }
}
