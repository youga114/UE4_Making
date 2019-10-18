// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Http.h"
#include "Json.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	Password = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Password")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));

	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::Connect);
	}
	MakeServerButton = Cast<UButton>(GetWidgetFromName(TEXT("MakeServerButton")));
	if (MakeServerButton)
	{
		MakeServerButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::MakeServer);
	}
}

void UTitleWidgetBase::Connect()
{
	if (ServerIP)
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI && UserID && Password)
		{
			GI->HTTPPost(TEXT("http://127.0.0.1:52273/login"), UserID->GetText().ToString(), Password->GetText().ToString(), FHttpRequestCompleteDelegate::CreateUObject(this, &UTitleWidgetBase::ConnectResponseReceived));
		}
	}
}

void UTitleWidgetBase::MakeServer()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI && UserID && Password)
	{
		GI->HTTPPost(TEXT("http://127.0.0.1:52273/login"), UserID->GetText().ToString(), Password->GetText().ToString(), FHttpRequestCompleteDelegate::CreateUObject(this, &UTitleWidgetBase::MakeServerResponseReceived));
	}
}

void UTitleWidgetBase::SetUserID()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->UserID = UserID->GetText().ToString();
	}
}

void UTitleWidgetBase::ConnectResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//JSON¿¡ ÆÄ½Ì
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString Result = JsonObject->GetStringField("Result");
			if (Result.Compare(TEXT("false")) != 0)
			{
				SetUserID();

				UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIP->GetText().ToString()));
			}
		}
	}
}

void UTitleWidgetBase::MakeServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//JSON¿¡ ÆÄ½Ì
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString Result = JsonObject->GetStringField("Result");
			if (Result.Compare(TEXT("false")) != 0)
			{
				SetUserID();

				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, TEXT("listen"));
			}
		}
	}
}
