// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagInsideWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Player/MyCharacter.h"

void UFlagInsideWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	RedImage = Cast<UImage>(GetWidgetFromName(TEXT("RedImage")));
	BlueImage = Cast<UImage>(GetWidgetFromName(TEXT("BlueImage")));
	WhiteImage = Cast<UImage>(GetWidgetFromName(TEXT("WhiteImage")));
	OccupationName = Cast<UTextBlock>(GetWidgetFromName(TEXT("OccupationName")));
	OccupationGaugeRed = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupationGaugeRed")));
	OccupationGaugeBlue = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupationGaugeBlue")));
	OccupationGaugeWhite = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupationGaugeWhite")));

	RedImage->SetVisibility(ESlateVisibility::Hidden);
	BlueImage->SetVisibility(ESlateVisibility::Hidden);
	WhiteImage->SetVisibility(ESlateVisibility::Hidden);

	AMyCharacter* Player = Cast<AMyCharacter>(GetOwningPlayerPawn());
	if (Player->ActorHasTag("Red"))					//첫 접속 게이지 방향 설정 이후 변동 X
	{
		OccupationGaugeRed->BarFillType = EProgressBarFillType::RightToLeft;
	}
	else
	{
		OccupationGaugeBlue->BarFillType = EProgressBarFillType::RightToLeft;
	}
}

void UFlagInsideWidgetBase::SetImageColor(FName FlagColor)
{
	if (FlagColor == "Red")
	{
		RedImage->SetVisibility(ESlateVisibility::Visible);
		BlueImage->SetVisibility(ESlateVisibility::Hidden);
		WhiteImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (FlagColor == "Blue")
	{
		RedImage->SetVisibility(ESlateVisibility::Hidden);
		BlueImage->SetVisibility(ESlateVisibility::Visible);
		WhiteImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RedImage->SetVisibility(ESlateVisibility::Hidden);
		BlueImage->SetVisibility(ESlateVisibility::Hidden);
		WhiteImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFlagInsideWidgetBase::SetOccupationName(FString Name)
{
	if (OccupationName)
	{
		OccupationName->SetText(FText::FromString(Name));
	}
}

void UFlagInsideWidgetBase::SetOccupationGauge(FName FlagColor)
{
	if (FlagColor == "Red")
	{
		OccupationGaugeRed->SetVisibility(ESlateVisibility::Visible);
		OccupationGaugeBlue->SetVisibility(ESlateVisibility::Hidden);
		OccupationGaugeWhite->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (FlagColor == "Blue")
	{
		OccupationGaugeRed->SetVisibility(ESlateVisibility::Hidden);
		OccupationGaugeBlue->SetVisibility(ESlateVisibility::Visible);
		OccupationGaugeWhite->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		OccupationGaugeRed->SetVisibility(ESlateVisibility::Hidden);
		OccupationGaugeBlue->SetVisibility(ESlateVisibility::Hidden);
		OccupationGaugeWhite->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFlagInsideWidgetBase::SetPercent(float Percent)
{
	OccupationGaugeRed->SetPercent(Percent);
	OccupationGaugeBlue->SetPercent(Percent);
	OccupationGaugeWhite->SetPercent(Percent);
}

void UFlagInsideWidgetBase::SetGaugeColor(FName WillFlagColor)
{
	OccupationGaugeWhite->FillColorAndOpacity = (WillFlagColor == "Red") ? FLinearColor::Red : FLinearColor::Blue;
}
