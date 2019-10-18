// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "Components/Button.h"

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		//#include "Path", 경로, UE4 Asset, BP
		FSoftClassPath LobbyWidgetClass_BP_PATH(TEXT("WidgetBlueprint'/Game/BluePrints/UI/Lobby/LobbyWidget.LobbyWidget_C'"));			//BP 경로 가져오기
	
		//#include "" 실행
		UClass* LobbyWidgetClass_BP = LobbyWidgetClass_BP_PATH.TryLoadClass<ULobbyWidgetBase>();

		if (LobbyWidgetClass_BP)
		{
			LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass_BP);
			if (LobbyWidget)
			{
				LobbyWidget->AddToViewport();
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());

				if (!HasAuthority())
				{
					LobbyWidget->StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

bool ALobbyPC::C2S_SendMessage_Validate(const FText & Message)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText & Message)
{
	for (auto iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*iter);
		if (PC)
		{
			PC->S2C_SendMessage(Message);
		}
	}
}

void ALobbyPC::S2C_SendMessage_Implementation(const FText & Message)
{
	LobbyWidget->AddMessage(Message);
}
