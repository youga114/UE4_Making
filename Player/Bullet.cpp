// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Player/MyPlayerController.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Zombie/AI/SoldierAIController.h"
#include "UnrealNetwork.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.0f);

	OnActorBeginOverlap.AddDynamic(this, &ABullet::OnBeginOverlap);

	beginLocation = GetActorLocation();

	AMyCharacter* Player = Cast<AMyCharacter>(GetOwner());
	if (Player)
	{
		APlayerController* PC = Player->GetController<APlayerController>();
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
			Movement->Velocity=ironVector * 4.25;
		}

		ASoldierAIController* AIC = Player->GetController<ASoldierAIController>();
		if (AIC)
		{
			AMyCharacter* Enemy = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
			if (Enemy)
			{
				float missRange = 30.0f;
				FVector destination = Enemy->GetActorLocation() + FVector(FMath::RandRange(-missRange, missRange), FMath::RandRange(-missRange, missRange), FMath::RandRange(-missRange, missRange));
				FVector ironVector = destination - GetActorLocation();
				Movement->Velocity = ironVector * 4.25;
			}
		}
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;

	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> IgnoreActors;

	FHitResult OutHit;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		beginLocation,
		GetActorLocation(),
		ObjectType,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f
	);

	beginLocation = GetActorLocation();

}

void ABullet::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)		//OverlappedActor: 액터 본인			OtherActor: 겹쳐진 액터
{
	AMyCharacter* Player = Cast<AMyCharacter>(GetOwner());
	if (Player)
	{
		Player->C2S_Shot(beginLocation, GetActorLocation(), this);
	}

	Destroy();
}
