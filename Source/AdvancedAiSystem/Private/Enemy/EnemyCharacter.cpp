
#include "Enemy/EnemyCharacter.h"
#include "Interface/EnemyControllerInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "BrainComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Component/DamageSystemActorComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsWeaponEquipped = false;
	EnemyCapsule = GetCapsuleComponent();
	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemActorComponent>(TEXT("DamageSystem"));

	EnemyMesh = GetMesh();
	EnemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
	EnemyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	EnemyMovementComponent = GetCharacterMovement();
	EnemyMovementComponent->GravityScale = 1.75f;
	EnemyMovementComponent->MaxAcceleration = 1500.0f;
	EnemyMovementComponent->BrakingFrictionFactor = 1.0f;
	EnemyMovementComponent->bUseSeparateBrakingFriction = true;
	EnemyMovementComponent->MaxWalkSpeed = 500.0f;
	EnemyMovementComponent->MinAnalogWalkSpeed = 20.0f;
	EnemyMovementComponent->BrakingDecelerationWalking = 2000.0f;
	EnemyMovementComponent->JumpZVelocity = 700.0f;
	EnemyMovementComponent->BrakingDecelerationFalling = 1500.0f;
	EnemyMovementComponent->AirControl = 0.35f;
	EnemyMovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	EnemyMovementComponent->bUseControllerDesiredRotation = true;
	EnemyMovementComponent->NavAgentProps.AgentRadius = 42.0f;
	EnemyMovementComponent->NavAgentProps.AgentHeight = 192.0f;
	EnemyMovementComponent->SetFixedBrakingDistance(200.0f);

}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = IEnemyControllerInterface::Execute_GetEnemyController(this->GetController());
	EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEnemyCharacter* AEnemyCharacter::EnemyRef_Implementation()
{
	return this;
}

void AEnemyCharacter::OnStealthTakenDown_Implementation(int SelectedMontageIndex)
{
	EnemyController->GetBrainComponent()->StopLogic(TEXT("Death"));
	EnemyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	EnemyMesh->GetAnimInstance()->Montage_Play(StealthVictimMontages[SelectedMontageIndex]);
}

//Patrolling(Issue called twice thrice etc)
void AEnemyCharacter::Patrolling_Implementation()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return;
	FVector RandomDestination;

	FNavLocation NavLocation;
	bool bRandomDestinationFound = NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 1200.0f, NavLocation);
	if (bRandomDestinationFound)
	{
		RandomDestination = NavLocation.Location;
		EnemyController->MoveToLocation(RandomDestination, 50.0f);
		EnemyController->GetPathFollowingComponent()->OnRequestFinished.Clear();
		EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddLambda([this](FAIRequestID RequestID, const FPathFollowingResult& Result)
			{
				if (Result.IsSuccess())
				{
					OnPatrollingDestinationReached.Broadcast();
					UE_LOG(LogTemp, Warning, TEXT("Move Succeeded! AI reached the destination."));
				}
				else
				{
					OnPatrollingDestinationReached.Broadcast();
					UE_LOG(LogTemp, Warning, TEXT("Move Failed! AI couldn't reach the destination."));
				}
			});
	}
}

void AEnemyCharacter::SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed, float& OutSpeed)
{
	switch (MovementSpeed)
	{
	case EMovementSpeed::Idle:
		OutSpeed = 0.0f;
		break;
	case EMovementSpeed::Walking:
		OutSpeed = 500.0f;
		break;
	case EMovementSpeed::Jogging:
		OutSpeed = 500.0f;
		break;
	case EMovementSpeed::Sprinting:
		OutSpeed = 600.0f;
		break;
	default:
		OutSpeed = 0.0f;
		break;
	}
}

void AEnemyCharacter::GetBattleRange_Implementation(float& OutAttackRadius, float& OutDefendRadius)
{
	OutAttackRadius = 600.0f;
	OutDefendRadius = 600.0f;
}

void AEnemyCharacter::Attack_Implementation()
{
	OnAttackEnd.Broadcast();
}

void AEnemyCharacter::ClearFocus_Implementation()
{
	EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
}

void AEnemyCharacter::EquipWeapon_Implementation()
{
	bIsWeaponEquipped = true;
	OnWeaponEquipped.Broadcast();
}

void AEnemyCharacter::UnequipWeapon_Implementation()
{
	bIsWeaponEquipped = false;
	OnWeaponUnequipped.Broadcast();
}

void AEnemyCharacter::JumpToDestination_Implementation(FVector Destination)
{
	const UObject* WorldContextObject = GetWorld();
	if (!WorldContextObject) return;
	FVector OutLaunchVelocity;
	FVector StartPosition = GetActorLocation();
	FVector EndPosition;
	EndPosition = Destination;
	EndPosition.Z = Destination.Z + 250.0f;
	bool bProjectileArcFound = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		WorldContextObject, OutLaunchVelocity, StartPosition, EndPosition);
	if (bProjectileArcFound)
	{
		LaunchCharacter(OutLaunchVelocity, true, true);
	}
}

float AEnemyCharacter::GetCurrentHealth_Implementation()
{
	return DamageSystemComponent->Health;
}

float AEnemyCharacter::GetMaxHealth_Implementation()
{
	return DamageSystemComponent->MaxHealth;
}

float AEnemyCharacter::Heal_Implementation(float Amount)
{
	return DamageSystemComponent->Heal(Amount);
}

bool AEnemyCharacter::TakeDamage_Implementation(FDamageInfo& DamageInfo)
{
	return DamageSystemComponent->TakeDamage(DamageInfo);
}

bool AEnemyCharacter::IsDead_Implementation()
{
	return DamageSystemComponent->bIsDead;
}

//Incomplete
void AEnemyCharacter::DamageResponse_Implementation(EDamageResponse& DamageResponse)
{

}

void AEnemyCharacter::Death_Implementation()
{
	EnemyMesh->SetSimulatePhysics(true);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EnemyController->GetBrainComponent()->StopLogic("Death");
	EnemyController->SetStateAsDead();
}


