
#include "Component/DamageSystemActorComponent.h"
#include "Structure/CommonStructs.h"


UDamageSystemActorComponent::UDamageSystemActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Health = 100.0f;
	MaxHealth = 100.0f;
	bIsDead = false;
}


void UDamageSystemActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UDamageSystemActorComponent::Heal(float HealAmount)
{
	if (!bIsDead)
	{
		Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
	}
	return Health;
}

bool UDamageSystemActorComponent::TakeDamage(FDamageInfo& DamageInfo)
{
	if (!bIsDead)
	{
		Health = Health - DamageInfo.Amount;
		if (Health <= 0.0f)
		{
			bIsDead = true;
			OnDeath.Broadcast();
			return true;
		}
		else
		{
			EDamageResponse DamageResponse = DamageInfo.DamageResponse;
			OnDamageResponse.Broadcast(DamageResponse);
			return true;
		};
	}
	else { return false; };
}


void UDamageSystemActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

