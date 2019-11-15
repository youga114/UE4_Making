// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Http.h"
#include "Json.h"

FString UMyGameInstance::GetUserID()
{
	return UserID;
}

void UMyGameInstance::HTTPPost(FString URL, FString UserID, FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
	//모듈 얻어오기
	Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	//콜백 등록
	Request->OnProcessRequestComplete() = ProcessRequestComplete;

	FString PostParameters = FString::Printf(TEXT("userid=%s"), *UserID) + FString::Printf(TEXT("&password=%s"), *Password);
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UMyGameInstance::HTTPGet()
{
	//모듈 얻어 오기
	Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMyGameInstance::HTTPResponseReceived);

	FString url = FString(TEXT("http://192.168.0.189:52273"));
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UMyGameInstance::HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GLog->Log(FString::Printf(TEXT("Callback")));
	if (bWasSuccessful)
	{
		//JSON 파싱
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		GLog->Log(FString::Printf(TEXT("Receive %s"), *Response->GetContentAsString()));

		//if (FJsonSerializer::Deserialize(Reader, JsonObject))
		//{
		//	FString ID = JsonObject->GetStringField("id");
		//	int Password = (int)JsonObject->GetNumberField("password");

		//	GLog->Log(FString::Printf(TEXT("Parsing %s %d"), *ID, Password));
		//}
	}
}
