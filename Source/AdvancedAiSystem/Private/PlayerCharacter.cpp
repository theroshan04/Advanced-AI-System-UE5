
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "Component/DamageSystemActorComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(FName(TEXT("PlayerCharacter")));

	PlayerCapsule = GetCapsuleComponent();
	PlayerCapsule->InitCapsuleSize(42.0f, 92.0f);

	PlayerMesh = GetMesh();
	PlayerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	PlayerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PlayerCapsule);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SocketOffset = FVector(0.0f, 60.0f, 40.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PlayerMovement = GetCharacterMovement();
	PlayerMovement->MaxWalkSpeed = 600.0f;

	DamageSystemComponent = CreateDefaultSubobject<UDamageSystemActorComponent>(TEXT("DamageSystem"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AMainPlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::StealthAttack()
{
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float APlayerCharacter::GetCurrentHealth_Implementation()
{
	return DamageSystemComponent->Health;
}

float APlayerCharacter::GetMaxHealth_Implementation()
{
	return DamageSystemComponent->MaxHealth;
}

float APlayerCharacter::Heal_Implementation(float Amount)
{
	return DamageSystemComponent->Heal(Amount);
}

bool APlayerCharacter::TakeDamage_Implementation(FDamageInfo& DamageInfo)
{
	return DamageSystemComponent->TakeDamage(DamageInfo);
}

bool APlayerCharacter::IsDead_Implementation()
{
	return DamageSystemComponent->bIsDead;
}

//Incomplete
void APlayerCharacter::DamageResponse_Implementation(EDamageResponse& DamageResponse)
{
}

void APlayerCharacter::Death_Implementation()
{
}

// Input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
	EnhancedInputComponent->BindAction(StealthAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::StealthAttack);

}

