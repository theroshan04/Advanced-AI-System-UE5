
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enumeration/CommonEnums.h"
#include "DamageSystemActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageResponse, EDamageResponse, DamageResponse);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ADVANCEDAISYSTEM_API UDamageSystemActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageSystemActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool TakeDamage(FDamageInfo& DamageInfo);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDamageResponse OnDamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

};
