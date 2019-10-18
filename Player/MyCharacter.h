// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"

#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class ESoldierState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Normal = 1 UMETA(DisplayName = "Normal"),
	Battle = 2 UMETA(DisplayName = "Battle"),
	Dead = 3 UMETA(DisplayName = "Dead"),
};

UCLASS()
class PRACTICE3_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JogSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 bIsIronsight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsFire : 1;


	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_LookUp(float Value);
	bool C2S_LookUp_Validate(float Value);
	void C2S_LookUp_Implementation(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_LookUp(float Value);
	void S2A_LookUp_Implementation(float Value);

	void Turn(float Value);

	void Sprint();
	void UnSprint();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Sprint();
	bool C2S_Sprint_Validate();
	void C2S_Sprint_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_UnSprint();
	bool C2S_UnSprint_Validate();
	void C2S_UnSprint_Implementation();

	void DoCrouch();

	void Ironsight();
	void UnIronsight();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Ironsight();
	bool C2S_Ironsight_Validate();
	void C2S_Ironsight_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_UnIronsight();
	bool C2S_UnIronsight_Validate();
	void C2S_UnIronsight_Implementation();

	void StartFire();
	void StopFire();

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Shot(FVector TraceStart, FVector TraceEnd, AActor* ignoreActor);
	bool C2S_Shot_Validate(FVector TraceStart, FVector TraceEnd, AActor* ignoreActor);
	void C2S_Shot_Implementation(FVector TraceStart, FVector TraceEnd, AActor* ignoreActor);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SpawnDecalAndEffect(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);
	void S2A_SpawnDecalAndEffect_Implementation(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SpawnSoundAndMuzzle();
	bool C2S_SpawnSoundAndMuzzle_Validate();
	void C2S_SpawnSoundAndMuzzle_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SpawnSoundAndMuzzle();
	void S2A_SpawnSoundAndMuzzle_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_ApplyPointDamage(FHitResult OutHit, FVector HitFromDirection);
	void S2A_ApplyPointDamage_Implementation(FHitResult OutHit, FVector HitFromDirection);

	FTimerHandle FireTimer;
	FTimerHandle DeadTimer;

	UFUNCTION()
	void FireTimerFunction();

	UFUNCTION()
	void DeadTimerFunction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BloodDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UCameraShake> FireCameraShake;

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector NormalSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CrouchedSpringArmPosition;

	UFUNCTION(BlueprintCallable)
	FVector GetSpringArmPosition() const;

	UFUNCTION(BlueprintCallable)
	void SetSpringArmPosition(FVector NewPosition);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* SoldierAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AActor> BulletClass;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, int32, Pawn);

	FSeePawnDelegate OnBeginPlay;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESoldierState CurrentState = ESoldierState::Normal;

	FRotator AimRotator;

	void SetAimRotator(FVector AimVector);

	void FindFlag();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;		//이거 없으면 3명 이상일 때 안됨.
};
