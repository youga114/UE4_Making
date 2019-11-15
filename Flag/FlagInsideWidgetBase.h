// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlagInsideWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API UFlagInsideWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UImage* RedImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UImage* BlueImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UImage* WhiteImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTextBlock* OccupationName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* OccupationGaugeRed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* OccupationGaugeBlue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* OccupationGaugeWhite;

	UFUNCTION(BlueprintCallable)
	void SetImageColor(FName FlagColor);

	UFUNCTION(BlueprintCallable)
	void SetOccupationName(FString Name);

	UFUNCTION(BlueprintCallable)
	void SetOccupationGauge(FName FlagColor);

	void SetPercent(float Percent);

	void SetGaugeColor(FName WillFlagColor);
};
