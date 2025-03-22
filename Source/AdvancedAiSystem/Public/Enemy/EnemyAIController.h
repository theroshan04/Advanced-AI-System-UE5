
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/EnemyControllerInterface.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UCLASS()
class ADVANCEDAISYSTEM_API AEnemyAIController : public AAIController, public IEnemyControllerInterface
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void BeginPlay() override;
	AEnemyAIController* GetEnemyController_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "SetState")
	EStateAI GetCurrentState() const;

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsPassive();

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsSeeking(FVector PointOfInterest);

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsInvestigating(FVector PointOfInterest);

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsAttacking(AActor* AttackTargetActor, bool bUseLastKnownAttackTarget);

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsFrozen();

	UFUNCTION(BlueprintCallable, Category = "SetState")
	void SetStateAsDead();

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void HandleSensedSight(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void HandleSensedDamage(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void HandleSensedHearing(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void HandleForgotActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void HandleLostSight(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void SeekAttackTarget();

	//Doubt
	UFUNCTION(BlueprintCallable, Category = "Sense")
	bool CanSenseActor(FAIStimulus& OutStimulus, AActor* Actor, ESenseAI Sense);

	//Incomplete
	UFUNCTION(BlueprintCallable, Category = "Sense")
	void CheckIfForgotSeenActor();

	UFUNCTION(BlueprintCallable, Category = "Sense")
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Reference", meta = (AllowPrivateAccess = "true"))
	AActor* AttackTarget;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|KeysName", meta = (AllowPrivateAccess = "true"))
	FName StateKeyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|KeysName", meta = (AllowPrivateAccess = "true"))
	FName AttackTargetKeyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|KeysName", meta = (AllowPrivateAccess = "true"))
	FName PointOfInterestKeyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|KeysName", meta = (AllowPrivateAccess = "true"))
	FName AttackRadiusKeyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|KeysName", meta = (AllowPrivateAccess = "true"))
	FName DefendRadiusKeyName;

	UPROPERTY(BlueprintReadWrite, Category = "Setup|Reference", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> KnownSeenActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Parameters", meta = (AllowPrivateAccess = "true"))
	float TimeToSeekAfterLoosingSight;

	UPROPERTY(BlueprintReadWrite, Category = "TimerHandle", meta = (AllowPrivateAccess = "true"))
	FTimerHandle CheckIfForgetSeenActorTimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "TimerHandle", meta = (AllowPrivateAccess = "true"))
	FTimerHandle SeekAttackTargetTimerHandle;



};

