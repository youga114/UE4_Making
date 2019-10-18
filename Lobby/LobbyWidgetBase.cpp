// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPC.h"
#include "MyGameInstance.h"
#include "Lobby/LobbyGM.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("PublicMessage")));
	ChattingBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChattingeBox")));
	StartGameButton = Cast<UButton>(GetWidgetFromName(TEXT("StartGameButton")));

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::StartGame);
	}

	if (ChatBox)
	{
		ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommit);
	}
}

void ULobbyWidgetBase::StartGame()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidgetBase::OnTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnCleared:
		{
			// 메시지 전송 후 삭제 되면 포커스
			ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PC)
			{
				ChatBox->SetUserFocus(PC);
			}
		}
		break;
		case ETextCommit::OnEnter:
		{
			if (!Text.IsEmpty())
			{
				//메시지 전송
				ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (PC && GI)
				{
					FString Temp = FString::Printf(TEXT("%s : %s"), *GI->GetUserID(), *Text.ToString());
					PC->C2S_SendMessage(FText::FromString(Temp));
					ChatBox->SetText(FText::FromString(TEXT("")));
				}
			}
			else
			{
				//채팅 끝
			}
		}
		break;
		case ETextCommit::OnUserMovedFocus:
		{
			//포커스 조정
		}
		break;
	}
}

void ULobbyWidgetBase::AddMessage(const FText & Text)
{
	if (ChattingBox)
	{
		UTextBlock* NewBlock = NewObject<UTextBlock>(ChattingBox);
		if (NewBlock)
		{
			NewBlock->SetText(Text);
			NewBlock->Font.Size = 18;
			ChattingBox->AddChild(NewBlock);
			ChattingBox->ScrollToEnd();				//스크롤 맨 아래로
		}
	}
}

void ULobbyWidgetBase::SetLeftTime(int32 LeftTime)
{
	if (PublicMessage)
	{
		FString Temp = FString::Printf(TEXT("%d초 남았습니다."), LeftTime);
		PublicMessage->SetText(FText::FromString(Temp));
	}
}

void ULobbyWidgetBase::SetConnectCount(int32 ConnectCount)
{
	if (AliveCount)
	{
		FString Temp = FString::Printf(TEXT("%d명 대기중"), ConnectCount);
		AliveCount->SetText(FText::FromString(Temp));
	}
}

void ULobbyWidgetBase::PlayStartButton_Implementation()
{
}
