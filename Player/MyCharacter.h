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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 bIsFire : 1;


	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_LookUp();
	bool C2S_LookUp_Validate();
	void C2S_LookUp_Implementation();

	//UFUNCTION(NetMulticast, Reliable)
	//void S2A_LookUp(float Value);
	//void S2A_LookUp_Implementation(float Value);

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

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_StartFire();
	bool C2S_StartFire_Validate();
	void C2S_StartFire_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_StopFire();
	bool C2S_StopFire_Validate();
	void C2S_StopFire_Implementation();

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SpawnBullet(FVector ironVector);
	bool C2S_SpawnBullet_Validate(FVector ironVector);
	void C2S_SpawnBullet_Implementation(FVector ironVector);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SpawnSoundAndMuzzle();
	bool C2S_SpawnSoundAndMuzzle_Validate();
	void C2S_SpawnSoundAndMuzzle_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SpawnSoundAndMuzzle();
	void S2A_SpawnSoundAndMuzzle_Implementation();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "ConnectCurrentHP_OnRep")
	float CurrentHP = 100.0f;

	UFUNCTION()
	void ConnectCurrentHP_OnRep();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AActor> PlayerClass;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, int32, Pawn);

	FSeePawnDelegate OnBeginPlay;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESoldierState CurrentState = ESoldierState::Normal;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FRotator AimRotator = FRotator::ZeroRotator;

	void SetAimRotator(FVector AimVector);

	void FindFlag();

	class AFlag* Flag = nullptr;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;		//이거 없으면 3명 이상일 때 안됨.

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SpawnCharacter();
	bool C2S_SpawnCharacter_Validate();
	void C2S_SpawnCharacter_Implementation();

	FTransform SpawnTransform;

	float ShakeHeight = 1.0f;

	UFUNCTION(NetMulticast, Reliable)			//NetMulticast: 이 함수는 서버 로컬 및 모든 클라이언트에 리플리케이트되는 상황 양쪽에서, 액터의 NetOwner 와 상관 없이 실행됩니다.
	void S2A_HitProcess();
	void S2A_HitProcess_Implementation();

	UFUNCTION(NetMulticast, Reliable)			//NetMulticast: 이 함수는 서버 로컬 및 모든 클라이언트에 리플리케이트되는 상황 양쪽에서, 액터의 NetOwner 와 상관 없이 실행됩니다.
	void S2A_DeadProcess();
	void S2A_DeadProcess_Implementation();
};
