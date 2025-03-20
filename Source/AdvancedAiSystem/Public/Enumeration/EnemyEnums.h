#pragma once

#include "CoreMinimal.h"
#include "EnemyEnums.generated.h"

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Jogging     UMETA(DisplayName = "Jogging"),
    Sprinting   UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class EStateAI : uint8
{
    Passive         UMETA(DisplayName = "Passive"),
    Seeking         UMETA(DisplayName = "Seeking"),
    Investigating   UMETA(DisplayName = "Investigating"),
    Attacking       UMETA(DisplayName = "Attacking"),
    Frozen          UMETA(DisplayName = "Frozen"),
    Dead            UMETA(DisplayName = "Dead")

};

UENUM(BlueprintType)
enum class ESenseAI : uint8
{
    None         UMETA(DisplayName = "None"),
    Sight        UMETA(DisplayName = "Sight"),
    Hearing      UMETA(DisplayName = "Hearing"),
    Damage       UMETA(DisplayName = "Damage")
};