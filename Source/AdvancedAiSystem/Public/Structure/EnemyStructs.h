#pragma once

#include "CoreMinimal.h"
#include "EnemyStructs.generated.h"

//Not in Use Struct
USTRUCT(BlueprintType)
struct FBattleRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefendRadius;
};
