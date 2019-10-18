// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

UCLASS()
class PRACTICE3_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetInitFlag(FName FlagColor);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_MultiChangeFlagColor(FName Color);
	void S2A_MultiChangeFlagColor_Implementation(FName Color);

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* RedBody;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlueBody;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* WhiteBody;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Boundary;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* RedBoundary;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlueBoundary;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* WhiteBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UArrowComponent* Arrow;

	FTimerHandle SpawnTimer;

	UFUNCTION()
	void SpawnTimerFunction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class APawn> RedClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBehaviorTree* RedTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class APawn> BlueClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBehaviorTree* BlueTree;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(ReplicatedUsing = "ChangeFlagColor_OnRep")				//������ �ִ� ���� Ŭ�� ���� ����
	FName FlagColor;

	float TeamCount = 1.0f;
	float EnemyCount = 1.0f;

	float BlueCount = 1.0f;
	float RedCount = 1.0f;

	UPROPERTY(ReplicatedUsing = "ChangeFlagPercent_OnRep")				//������ �ִ� ���� Ŭ�� ���� ����(���� �ٲ�� �ڵ� ConnectCount_OnRepȣ��)
	float Percent = 0.0f;

	UFUNCTION()
	void ChangeFlagPercent_OnRep();

	UFUNCTION()
	void ChangeFlagColor_OnRep();

	UFUNCTION()
	void PercentTimer();

	FTimerHandle PercentTimerHandle;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
