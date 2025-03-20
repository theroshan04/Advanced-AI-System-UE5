
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structure/CommonStructs.h"
#include "Enumeration/CommonEnums.h"
#include "DamageSystemInterface.generated.h"


UINTERFACE(MinimalAPI)
class UDamageSystemInterface : public UInterface
{
	GENERATED_BODY()
};


class ADVANCEDAISYSTEM_API IDamageSystemInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	float GetCurrentHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	float GetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	float Heal(float Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	bool TakeDamage(FDamageInfo& DamageInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	void DamageResponse(EDamageResponse& DamageResponse);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageSystem")
	void Death();
};
