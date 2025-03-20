
#include "Enemy/EnemyAIController.h"
#include "Enumeration/EnemyEnums.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	//SightSense
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;
	SightConfig->SetMaxAge(15.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//HearingSense
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.0f;
	HearingConfig->SetMaxAge(2.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//DamageSense
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(1.0f);

	//ConfigureSenses
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->ConfigureSense(*DamageConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	//Variables
	StateKeyName = FName(TEXT("State"));
	AttackTargetKeyName = FName(TEXT("AttackTarget"));
	PointOfInterestKeyName = FName(TEXT("PointOfInterest"));
	AttackRadiusKeyName = FName(TEXT("AttackRadius"));
	DefendRadiusKeyName = FName(TEXT("DefendRadius"));
	TimeToSeekAfterLoosingSight = 3.0f;

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

AEnemyAIController* AEnemyAIController::GetEnemyController_Implementation()
{
	return this;
}

EStateAI AEnemyAIController::GetCurrentState() const
{
	EStateAI CurrentState;
	CurrentState = static_cast<EStateAI>(Blackboard->GetValueAsEnum(StateKeyName));
	return CurrentState;
}

void AEnemyAIController::SetStateAsPassive()
{
	Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Passive));
}

void AEnemyAIController::SetStateAsSeeking(FVector PointOfInterest)
{
	Blackboard->SetValueAsVector(PointOfInterestKeyName, PointOfInterest);
	Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Seeking));
}

void AEnemyAIController::SetStateAsInvestigating(FVector PointOfInterest)
{
	Blackboard->SetValueAsVector(PointOfInterestKeyName, PointOfInterest);
	Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Investigating));
}

void AEnemyAIController::SetStateAsAttacking(AActor* AttackTargetActor, bool bUseLastKnownAttackTarget)
{
	AActor* NewAttackTarget;
	NewAttackTarget = (AttackTarget && bUseLastKnownAttackTarget) ? AttackTarget : AttackTargetActor;
	if (NewAttackTarget)
	{
		Blackboard->SetValueAsObject(AttackTargetKeyName, NewAttackTarget);
		Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Attacking));
		AttackTarget = NewAttackTarget;
	}
	else { SetStateAsPassive(); }
}

void AEnemyAIController::SetStateAsFrozen()
{
	Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Frozen));
}

void AEnemyAIController::SetStateAsDead()
{
	Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(EStateAI::Dead));
}

void AEnemyAIController::HandleSensedSight(AActor* Actor)
{
	KnownSeenActor.AddUnique(Actor);
	EStateAI EState = GetCurrentState();
	switch (EState)
	{
	case EStateAI::Passive:
	case EStateAI::Seeking:
	case EStateAI::Investigating:

		if (Actor->ActorHasTag(FName(TEXT("PlayerCharacter"))))
		{
			SetStateAsAttacking(Actor, false);
		}
		break;

	case EStateAI::Attacking:

		if (SeekAttackTargetTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);
			SeekAttackTargetTimerHandle.Invalidate();
		}
		break;

	case EStateAI::Frozen:
	case EStateAI::Dead:
		break;
	default:
		break;
	}
}

void AEnemyAIController::HandleSensedDamage(AActor* Actor)
{
	EStateAI EState = GetCurrentState();
	switch (EState)
	{
	case EStateAI::Passive:
	case EStateAI::Seeking:
	case EStateAI::Investigating:
		SetStateAsAttacking(Actor, false);
		break;

	case EStateAI::Attacking:
	case EStateAI::Frozen:
	case EStateAI::Dead:
		break;
	default:
		break;
	}
}

void AEnemyAIController::HandleSensedHearing(FVector Location)
{
	EStateAI EState = GetCurrentState();
	switch (EState)
	{
	case EStateAI::Passive:
	case EStateAI::Seeking:
	case EStateAI::Investigating:
		SetStateAsInvestigating(Location);
		break;
	case EStateAI::Attacking:
	case EStateAI::Frozen:
	case EStateAI::Dead:
		break;
	default:
		break;
	}
}

void AEnemyAIController::HandleForgotActor(AActor* Actor)
{
	KnownSeenActor.Remove(Actor);
	if (Actor == AttackTarget)
	{
		SetStateAsPassive();
	}
}

void AEnemyAIController::HandleLostSight(AActor* Actor)
{
	if (Actor == AttackTarget)
	{
		EStateAI EState = GetCurrentState();
		switch (EState)
		{
		case EStateAI::Investigating:
		case EStateAI::Attacking:
		case EStateAI::Frozen:
			if (SeekAttackTargetTimerHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);
				SeekAttackTargetTimerHandle.Invalidate();
			}
			GetWorldTimerManager().SetTimer(SeekAttackTargetTimerHandle, this, &AEnemyAIController::SeekAttackTarget, TimeToSeekAfterLoosingSight, false);
			break;

		case EStateAI::Passive:
		case EStateAI::Seeking:
		case EStateAI::Dead:
			break;
		default:
			break;
		}
	}
}

void AEnemyAIController::SeekAttackTarget()
{
	SetStateAsSeeking(AttackTarget->GetActorLocation());
	if (SeekAttackTargetTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);
		SeekAttackTargetTimerHandle.Invalidate();
	}
}

bool AEnemyAIController::CanSenseActor(FAIStimulus& OutStimulus, AActor* Actor, ESenseAI Sense)
{
	if (!Actor || !AIPerception) return false;
	FActorPerceptionBlueprintInfo Info;
	AIPerception->GetActorsPerception(Actor, Info);
	TArray<FAIStimulus> LastSensedStimuli = Info.LastSensedStimuli;
	for (const FAIStimulus& Stimulus : LastSensedStimuli)
	{
		TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
		if ((Sense == ESenseAI::Sight && SenseClass == UAISense_Sight::StaticClass()) ||
			(Sense == ESenseAI::Hearing && SenseClass == UAISense_Hearing::StaticClass()) ||
			(Sense == ESenseAI::Damage && SenseClass == UAISense_Damage::StaticClass()))
		{
			OutStimulus = Stimulus;
			return Stimulus.WasSuccessfullySensed();
		}
	}
	return false;
}

void AEnemyAIController::CheckIfForgotSeenActor()
{

}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	FAIStimulus OutStimulus_Sight;
	FAIStimulus OutStimulus_Hearing;
	FAIStimulus OutStimulus_Damage;

	for (AActor* Actor : UpdatedActors)
	{
		if (CanSenseActor(OutStimulus_Sight, Actor, ESenseAI::Sight))
		{
			HandleSensedSight(Actor);
		}
		else { HandleLostSight(Actor); }

		if (CanSenseActor(OutStimulus_Hearing, Actor, ESenseAI::Hearing))
		{
			HandleSensedHearing(OutStimulus_Hearing.StimulusLocation);
		}

		if (CanSenseActor(OutStimulus_Damage, Actor, ESenseAI::Damage))
		{
			HandleSensedDamage(Actor);
		}
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* EnemyRef = IEnemyInterface::Execute_EnemyRef(InPawn);
	RunBehaviorTree(EnemyRef->BehaviorTree);
	SetStateAsPassive();
	float AttackRadius;
	float DefendRadius;
	IEnemyInterface::Execute_GetBattleRange(InPawn, AttackRadius, DefendRadius);
	Blackboard->SetValueAsFloat(AttackRadiusKeyName, AttackRadius);
	Blackboard->SetValueAsFloat(DefendRadiusKeyName, DefendRadius);
	GetWorldTimerManager().SetTimer(CheckIfForgetSeenActorTimerHandle, this, &AEnemyAIController::CheckIfForgotSeenActor, 0.5, true);
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (CheckIfForgetSeenActorTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(CheckIfForgetSeenActorTimerHandle);
		CheckIfForgetSeenActorTimerHandle.Invalidate();
	}
}



