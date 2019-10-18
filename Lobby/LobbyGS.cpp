// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "UnrealNetwork.h"
#include "Lobby/LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "Kismet/GameplayStatics.h"

ALobbyGS::ALobbyGS()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyGS::ConnectCount_OnRep()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)									//PostLogin()이 실행된 뒤 PC에서 BeginPlay()가 실행 되므로 LobbyWidget이 없을 때 함수를 불러서 에러가 나기 때문에 검사
	{
		PC->LobbyWidget->SetConnectCount(ConnectCount);			// UPROPERTY(ReplicatedUsing="ConnectCount_OnRep") 을 호출했기 때문에 틱마다 이짓을 안해도 됨
	}
}

void ALobbyGS::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetLeftTime(LeftTime);
		//PC->LobbyWidget->SetAlive(Alive);						// UPROPERTY(ReplicatedUsing="ConnectCount_OnRep") 을 호출했기 때문에 틱마다 이짓을 안해도 됨
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);					// 네트워크로 정보 보내주는 포트 생성
	DOREPLIFETIME(ALobbyGS, ConnectCount);				// 네트워크로 정보 보내주는 포트 생성
}
