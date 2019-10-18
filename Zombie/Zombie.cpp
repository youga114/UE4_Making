// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/MyCharacter.h"
#include "Zombie/AI/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Tags.Add(TEXT("Character"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	PawnSensing->OnSeePawn.AddDynamic(this, &AZombie::OnSeePawn);
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP == 0)
	{
		return 0;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("Head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}

	PlayAnimMontage(HitAnimation, 2.0f, FName(TEXT("Hit")));

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, 100);

	if (CurrentHP == 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PlayAnimMontage(DeadAnimation, 1.0f, FName(TEXT("Death")));
		CurrentState = EZombieState::Dead;
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZombie::OnSeePawn(APawn * Pawn)
{
	if (CurrentState == EZombieState::Normal && Pawn->ActorHasTag(TEXT("Player")))
	{
		CurrentState = EZombieState::Chase;
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
			AIC->SetTargetPlayer(Pawn);
		}
	}
}

void AZombie::Attack()
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
		if (Player)
		{
			UGameplayStatics::ApplyDamage(Player, 30.0f, GetController(), this, nullptr);
		}
	}
}
