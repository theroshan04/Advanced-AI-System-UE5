#pragma once

#include "CoreMinimal.h"
#include "Enumeration/CommonEnums.h"
#include "CommonStructs.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse DamageResponse;

};