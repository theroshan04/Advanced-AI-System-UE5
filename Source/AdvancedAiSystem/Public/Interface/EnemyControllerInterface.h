
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyControllerInterface.generated.h"

class AEnemyAIController;

UINTERFACE(MinimalAPI)
class UEnemyControllerInterface : public UInterface
{
	GENERATED_BODY()
};


class ADVANCEDAISYSTEM_API IEnemyControllerInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyControllerInterface")
	AEnemyAIController* GetEnemyController();
};
