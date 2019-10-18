// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Normal = 1 UMETA(DisplayName = "Normal"),
	Chase = 2 UMETA(DisplayName = "Chase"),
	Battle = 3 UMETA(DisplayName = "Battle"),
	Dead = 4 UMETA(DisplayName = "Dead"),
};

UCLASS()
class PRACTICE3_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EZombieState CurrentState = EZombieState::Normal;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* ZombieAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* PawnSensing;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, int32, Pawn);
	
	FSeePawnDelegate OnBeginPlay;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	float WalkSpeed = 150.0f;
	float RunSpeed = 300.0f;

	void Attack();
};
