// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePC.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API ABattlePC : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UFlagInsideWidgetBase> FlagInsideWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UFlagInsideWidgetBase* FlagInsideWidget;

	void SetAndShowFlagWidget(FName FlagColor);
	void HideFlagBoundary();

	void SetPercent(float Percent);

	AActor* Flag = nullptr;
};
