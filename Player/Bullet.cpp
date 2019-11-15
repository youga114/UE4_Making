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
	PrimaryActorTick.bCanEverTick = false;

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
}

void ABullet::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)		//OverlappedActor: 액터 본인			OtherActor: 겹쳐진 액터
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;

	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<FHitResult> OutHits;

	bool bResult = UKismetSystemLibrary::LineTraceMultiForObjects(
		GetWorld(),
		beginLocation,
		GetActorLocation(),
		ObjectType,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);

	if (bResult)
	{
		for (auto OutHit : OutHits)
		{
			if (OutHit.GetActor() == OtherActor)
			{
				UParticleSystem* Hit;
				UMaterialInterface* DecalP;
				if (OutHit.GetActor()->ActorHasTag(TEXT("Character")))
				{
					Hit = BloodEffect;
					DecalP = BloodDecal;
				}
				else
				{
					Hit = HitEffect;
					DecalP = BulletDecal;
				}

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit, OutHit.ImpactPoint + OutHit.ImpactNormal*5.0f, OutHit.ImpactNormal.Rotation());

				UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalP, FVector(3, 3, 3), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation(), 10.0f);
				Decal->SetFadeScreenSize(0.001f);

				S2A_SpawnDecalAndEffect(OutHit, Hit, DecalP);

				UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), 10.0f, GetActorLocation() - beginLocation, OutHit, OutHit.GetActor()->GetInstigatorController(), GetOwner(), nullptr);

				break;
			}
		}
	}

	Destroy();
}

void ABullet::S2A_SpawnDecalAndEffect_Implementation(FHitResult OutHit, UParticleSystem * Hit, UMaterialInterface * DecalP)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit, OutHit.ImpactPoint + OutHit.ImpactNormal*5.0f, OutHit.ImpactNormal.Rotation());
	
	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalP, FVector(3, 3, 3), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation(), 10.0f);
	Decal->SetFadeScreenSize(0.001f);
}
