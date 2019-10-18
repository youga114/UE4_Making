// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTextBlock* AliveCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTextBlock* PublicMessage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UScrollBox* ChattingBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UEditableTextBox* ChatBox;

	UPROPERTY(BLueprintReadOnly, EditAnywhere)
	class UButton* StartGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable)
	void AddMessage(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void SetLeftTime(int32 LeftTime);					//�ð� UI �ٲ��ִ� �Լ�

	UFUNCTION(BlueprintCallable)
	void SetConnectCount(int32 ConnectCount);				//Alive UI �ٲ��ִ� �Լ�

	//C++���� �����ϰ� BP���� ����, ����� �Ѵ�.
	UFUNCTION(BlueprintNativeEvent)
	void PlayStartButton();
	void PlayStartButton_Implementation();
};
