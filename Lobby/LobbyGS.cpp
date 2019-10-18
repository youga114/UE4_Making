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
	if (PC && PC->LobbyWidget)									//PostLogin()�� ����� �� PC���� BeginPlay()�� ���� �ǹǷ� LobbyWidget�� ���� �� �Լ��� �ҷ��� ������ ���� ������ �˻�
	{
		PC->LobbyWidget->SetConnectCount(ConnectCount);			// UPROPERTY(ReplicatedUsing="ConnectCount_OnRep") �� ȣ���߱� ������ ƽ���� ������ ���ص� ��
	}
}

void ALobbyGS::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		PC->LobbyWidget->SetLeftTime(LeftTime);
		//PC->LobbyWidget->SetAlive(Alive);						// UPROPERTY(ReplicatedUsing="ConnectCount_OnRep") �� ȣ���߱� ������ ƽ���� ������ ���ص� ��
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);					// ��Ʈ��ũ�� ���� �����ִ� ��Ʈ ����
	DOREPLIFETIME(ALobbyGS, ConnectCount);				// ��Ʈ��ũ�� ���� �����ִ� ��Ʈ ����
}
