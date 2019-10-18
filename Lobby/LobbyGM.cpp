// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Lobby/LobbyGS.h"

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LeftTimerHandle, this, &ALobbyGM::LeftTimer, 1.0f, true, 1.0f);
}

void ALobbyGM::LeftTimer()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();				//���� ���ص� �̹� ����
	if (GS)
	{
		GS->LeftTime--;
		if (GS->LeftTime <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(LeftTimerHandle);
			StartGame();
		}
	}
}

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGS* GS = GetGameState<ALobbyGS>();				//���� ���ص� �̹� ����
	if (GS)
	{
		GS->ConnectCount++;

		if (HasAuthority() && GetNetMode() == ENetMode::NM_ListenServer)		//������ �����
		{
			GS->ConnectCount_OnRep();			//������ ���� �ٲ��� ���ϹǷ� ȣ���ؾ���.
		}
	}
}

void ALobbyGM::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	ALobbyGS* GS = GetGameState<ALobbyGS>();							//���� ���ص� �̹� ����
	if (GS)
	{
		GS->ConnectCount--;

		if (HasAuthority())						//������ �����
		{
			GS->ConnectCount_OnRep();			// ������ ���� �ٲ��� ���ϹǷ� ȣ���ؾ���.
		}
	}
}

void ALobbyGM::StartGame()
{
	GetWorld()->ServerTravel(TEXT("Battle"));
}
