// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PRACTICE3_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Movement;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector beginLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BloodDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SpawnDecalAndEffect(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);
	void S2A_SpawnDecalAndEffect_Implementation(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);

	AActor* Creator;
};
