// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "Flag/FlagInsideWidgetBase.h"
#include "Player/MyCharacter.h"

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())				//위젯 초기화
	{
		FlagInsideWidget = CreateWidget<UFlagInsideWidgetBase>(this, FlagInsideWidgetClass);
		FlagInsideWidget->AddToViewport();

		AMyCharacter* Player = Cast<AMyCharacter>(GetPawn());		//첫 접속 위젯 초기화
		if (Player)
		{
			if (Player->ActorHasTag("Red"))
			{
				SetAndShowFlagWidget("Red");
			}
			else
			{
				SetAndShowFlagWidget("Blue");
			}
		}
	}
}

void ABattlePC::SetAndShowFlagWidget(FName FlagColor)
{
	if (FlagInsideWidget)
	{
		FlagInsideWidget->SetVisibility(ESlateVisibility::Visible);
		FlagInsideWidget->SetImageColor(FlagColor);
		FlagInsideWidget->SetOccupationGauge(FlagColor);
	}
}

void ABattlePC::HideFlagBoundary()
{
	if (FlagInsideWidget)
	{
		FlagInsideWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABattlePC::SetPercent(float Percent)
{
	FlagInsideWidget->SetPercent(Percent);
}

