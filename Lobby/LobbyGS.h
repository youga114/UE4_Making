// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)			//������ �ִ� ���� Ŭ�� ���� ����
	int LeftTime = 60;

	UPROPERTY(ReplicatedUsing = "ConnectCount_OnRep")			//������ �ִ� ���� Ŭ�� ���� ����(���� �ٲ�� �ڵ� ConnectCount_OnRepȣ��)
	int ConnectCount = 0;

	UFUNCTION()
	void ConnectCount_OnRep();

	ALobbyGS();

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
