#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None         UMETA(DisplayName = "None"),
	Melee        UMETA(DisplayName = "Melee"),
	Ranged       UMETA(DisplayName = "Ranged"),
	Explosion    UMETA(DisplayName = "Explosion"),
	Environment  UMETA(DisplayName = "Environment")
};

UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	None UMETA(DisplayName = "None"),
	Default UMETA(DisplayName = "Default")
};