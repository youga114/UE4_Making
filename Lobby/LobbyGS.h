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
	UPROPERTY(Replicated)			//서버에 있는 값을 클라에 전부 복사
	int LeftTime = 60;

	UPROPERTY(ReplicatedUsing = "ConnectCount_OnRep")			//서버에 있는 값을 클라에 전부 복사(값이 바뀌면 자동 ConnectCount_OnRep호출)
	int ConnectCount = 0;

	UFUNCTION()
	void ConnectCount_OnRep();

	ALobbyGS();

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
