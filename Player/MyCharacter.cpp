// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Zombie/AI/SoldierAIController.h"
#include "UnrealNetwork.h"
#include "Bullet.h"
#include "Flag.h"
#include "Battle/BattlePC.h"
#include "Battle/BattleWidgetBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Tags.Add(TEXT("Character"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	NormalSpringArmPosition = GetSpringArmPosition();
	CrouchedSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -44.0f);

	CurrentHP = MaxHP;

	PawnSensing->OnSeePawn.AddDynamic(this, &AMyCharacter::OnSeePawn);

	SpawnTransform = GetActorTransform();

	FindFlag();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::UnSprint);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::DoCrouch);

	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &AMyCharacter::Ironsight);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &AMyCharacter::UnIronsight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::StopFire);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AMyCharacter::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);

		C2S_LookUp();
	}
}

bool AMyCharacter::C2S_LookUp_Validate()
{
	return true;
}

void AMyCharacter::C2S_LookUp_Implementation()
{
	AimRotator = GetControlRotation();			// 0 ~ 360
}

void AMyCharacter::Turn(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}

void AMyCharacter::Sprint()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	C2S_Sprint();
}

void AMyCharacter::UnSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;

	C2S_UnSprint();
}

bool AMyCharacter::C2S_Sprint_Validate()
{
	return true;
}

void AMyCharacter::C2S_Sprint_Implementation()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

bool AMyCharacter::C2S_UnSprint_Validate()
{
	return true;
}

void AMyCharacter::C2S_UnSprint_Implementation()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void AMyCharacter::DoCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::Ironsight()
{
	if (!bIsIronsight)
	{
		bIsIronsight = true;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

		C2S_Ironsight();
	}
}

void AMyCharacter::UnIronsight()
{
	bIsIronsight = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	C2S_UnIronsight();
}

bool AMyCharacter::C2S_Ironsight_Validate()
{
	return true;
}

void AMyCharacter::C2S_Ironsight_Implementation()
{
	bIsIronsight = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}

bool AMyCharacter::C2S_UnIronsight_Validate()
{
	return true;
}

void AMyCharacter::C2S_UnIronsight_Implementation()
{
	bIsIronsight = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMyCharacter::StartFire()
{
	if (!bIsFire)
	{
		bIsFire = true;

		C2S_StartFire();

		Fire();
	}
}

void AMyCharacter::StopFire()
{
	bIsFire = false;

	C2S_StopFire();
}

bool AMyCharacter::C2S_StartFire_Validate()
{
	return true;
}

void AMyCharacter::C2S_StartFire_Implementation()
{
	bIsFire = true;
}

bool AMyCharacter::C2S_StopFire_Validate()
{
	return true;
}

void AMyCharacter::C2S_StopFire_Implementation()
{
	bIsFire = false;
}

void AMyCharacter::Fire()
{
	if (!bIsFire)
	{
		return;
	}

	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		int32 SizeX;
		int32 SizeY;
		FVector WorldLocation;
		FVector WorldDirection;

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		PC->GetViewportSize(SizeX, SizeY);
		PC->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);
		FVector centerLocation = WorldLocation + (WorldDirection * 2000.0f);
		FVector ironVector = centerLocation - GetActorLocation();
		ironVector *= 4.25;

		C2S_SpawnBullet(ironVector);						//총알 생성(서버가)

		PC->PlayerCameraManager->PlayCameraShake(FireCameraShake);			//카메라 흔들기
	}

	ASoldierAIController* AIC = GetController<ASoldierAIController>();
	if (AIC)
	{
		AMyCharacter* Enemy = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
		if (Enemy)
		{
			float missRange = 30.0f;
			FVector destination = Enemy->GetActorLocation() + FVector(FMath::RandRange(-missRange, missRange), FMath::RandRange(-missRange, missRange), FMath::RandRange(-missRange, missRange));
			FVector ironVector = destination - GetActorLocation();
			ironVector *= 4.25;

			C2S_SpawnBullet(ironVector);						//총알 생성(서버가)
		}
	}

	FRotator PlayerRotation = GetControlRotation();
	PlayerRotation.Pitch += ShakeHeight;
	GetController()->SetControlRotation(PlayerRotation);
	C2S_LookUp();

	C2S_SpawnSoundAndMuzzle();				//소리, 이펙트 생성

	if (bIsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AMyCharacter::FireTimerFunction, 0.12f);					//0.12초 후에 함수를 호출함
	}
}

bool AMyCharacter::C2S_SpawnBullet_Validate(FVector ironVector)
{
	return true;
}

void AMyCharacter::C2S_SpawnBullet_Implementation(FVector ironVector)
{
	ABullet* Bullet = Cast<ABullet>(GetWorld()->SpawnActor<AActor>(BulletClass, Weapon->GetSocketTransform(TEXT("Muzzle"))));
	if (Bullet)
	{
		UProjectileMovementComponent* BulletMovement = Cast<UProjectileMovementComponent>(Bullet->Movement);
		if (BulletMovement)
		{
			BulletMovement->Velocity = ironVector;
		}
	}
}

bool AMyCharacter::C2S_SpawnSoundAndMuzzle_Validate()
{
	return true;
}

void AMyCharacter::C2S_SpawnSoundAndMuzzle_Implementation()
{
	S2A_SpawnSoundAndMuzzle();
}

void AMyCharacter::S2A_SpawnSoundAndMuzzle_Implementation()
{
	if (FireSound && MuzzleFlash)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")), Weapon->GetSocketRotation(TEXT("Muzzle")));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Weapon->GetSocketLocation(TEXT("Muzzle")), Weapon->GetSocketRotation(TEXT("Muzzle")));
	}
}

void AMyCharacter::FireTimerFunction()
{
	Fire();
}

void AMyCharacter::DeadTimerFunction()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC && PC->BattleWidget)
	{
		PC->BattleWidget->SetHp(1.0f);				//위젯 체력바 초기화

		C2S_SpawnCharacter();
	}
}

FRotator AMyCharacter::GetAimOffset() const
{
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
	}

	FRotator ret = AimRotator;			// 0 ~ 360
	if (ret.Pitch > 180.0f)
	{
		ret.Pitch -= 360.0f;				//AimPitch 값을 -90 ~ 90로 하기 위하여
	}

	return ret;
}

FVector AMyCharacter::GetSpringArmPosition() const
{
	return SpringArm->GetRelativeTransform().GetLocation();
}

void AMyCharacter::SetSpringArmPosition(FVector NewPosition)
{
	SpringArm->SetRelativeLocation(NewPosition);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)				//서버에서 실행되는 함수
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
	else
	{
		CurrentHP -= DamageAmount;
	}

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);
	if (HasAuthority())
	{
		ConnectCurrentHP_OnRep();
	}

	S2A_HitProcess();

	//AMyCharacter* Someone = Cast<AMyCharacter>(DamageCauser);
	//if (ActorHasTag(TEXT("Red")))					//내 팀을 먼저 확인하고
	//{
	//	if (Someone && CurrentState == ESoldierState::Normal && Someone->ActorHasTag(TEXT("Blue")))		//적이 쏜것이라면
	//	{
	//		ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
	//		if (AIC && CurrentState == ESoldierState::Normal)
	//		{
	//			CurrentState = ESoldierState::Battle;
	//			AIC->SetCurrentState(CurrentState);
	//			AIC->SetTargetPlayer(Someone);
	//		}
	//	}
	//}
	//else if (ActorHasTag(TEXT("Blue")))
	//{
	//	if (Someone && CurrentState == ESoldierState::Normal && Someone->ActorHasTag(TEXT("Red")))
	//	{
	//		ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
	//		if (AIC && CurrentState == ESoldierState::Normal)
	//		{
	//			CurrentState = ESoldierState::Battle;
	//			AIC->SetCurrentState(CurrentState);
	//			AIC->SetTargetPlayer(Someone);
	//		}
	//	}
	//}

	if (CurrentHP == 0)
	{
		S2A_DeadProcess();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyCharacter::ConnectCurrentHP_OnRep()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC && PC->BattleWidget)								//서버의 경우 모든 클라이언트의 PC는 가지고 있지만 Widget은 가지고 있지 않음
	{
		PC->BattleWidget->SetHp(CurrentHP / 100);
	}
}

void AMyCharacter::OnSeePawn(APawn * Pawn)				//한번만 실행됨
{
	AMyCharacter* Someone = Cast<AMyCharacter>(Pawn);
	if (!Someone)
	{
		return;
	}

	if (ActorHasTag(TEXT("Red")))
	{
		if (CurrentState == ESoldierState::Normal && Someone->ActorHasTag(TEXT("Blue")) && Someone->CurrentHP > 0)
		{
			ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
			if (AIC)
			{
				CurrentState = ESoldierState::Battle;
				AIC->SetCurrentState(CurrentState);
				AIC->SetTargetPlayer(Someone);
			}
		}
	}
	else if (ActorHasTag(TEXT("Blue")))
	{
		if (CurrentState == ESoldierState::Normal && Someone->ActorHasTag(TEXT("Red")) && Someone->CurrentHP > 0)
		{
			ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
			if (AIC)
			{
				CurrentState = ESoldierState::Battle;
				AIC->SetCurrentState(CurrentState);
				AIC->SetTargetPlayer(Someone);
			}
		}
	}
}

void AMyCharacter::SetAimRotator(FVector AimVector)
{
	AimRotator = AimVector.Rotation();
}

void AMyCharacter::FindFlag()
{
	ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
	if (AIC)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Flag"), OutActors);

		if (OutActors.Num() > 0)
		{
			AIC->SetTargetFlag(OutActors[FMath::RandRange(0, OutActors.Num() - 1)]);				//AI가 도착할 Flag설정
		}
	}
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, bIsSprint);
	DOREPLIFETIME(AMyCharacter, bIsIronsight);
	DOREPLIFETIME(AMyCharacter, AimRotator);
	DOREPLIFETIME(AMyCharacter, CurrentHP);
	DOREPLIFETIME(AMyCharacter, bIsFire);
}

bool AMyCharacter::C2S_SpawnCharacter_Validate()
{
	return true;
}

void AMyCharacter::C2S_SpawnCharacter_Implementation()
{
	ABattlePC* PC = Cast<ABattlePC>(GetController());
	if (PC)
	{
		Destroy();
		PC->UnPossess();																						//언포세스 서버에서
		AMyCharacter* MyCharacter = GetWorld()->SpawnActor<AMyCharacter>(PlayerClass, SpawnTransform);			//스폰 서버에서
		PC->Possess(MyCharacter);																				//포세스 서버에서
	}
}

void AMyCharacter::S2A_HitProcess_Implementation()
{
	FString HitName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 4));

	PlayAnimMontage(HitAnimation, 2.0f, FName(*HitName));
}

void AMyCharacter::S2A_DeadProcess_Implementation()
{
	FString DeadName = FString::Printf(TEXT("death_%d"), FMath::RandRange(1, 3));

	PlayAnimMontage(DeadAnimation, 1.0f, FName(*DeadName));

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		StopFire();
		UnIronsight();
		DisableInput(PC);					//입력 전부 막기
	}

	ASoldierAIController* AIC = Cast<ASoldierAIController>(GetController());
	if (AIC)
	{
		CurrentState = ESoldierState::Dead;
		AIC->SetCurrentState(ESoldierState::Dead);
		StopFire();
		UnIronsight();
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorldTimerManager().SetTimer(DeadTimer, this, &AMyCharacter::DeadTimerFunction, 5.0f);
}

//애니메이션이 클라이언트마다 다르므로 TakeDamage로 뿌릴때 같은 애니메이션 적용
//데드 애니메이션도 마찬가지