// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "Flag/FlagInsideWidgetBase.h"
#include "Player/MyCharacter.h"

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())				//���� �ʱ�ȭ
	{
		FlagInsideWidget = CreateWidget<UFlagInsideWidgetBase>(this, FlagInsideWidgetClass);
		FlagInsideWidget->AddToViewport();

		AMyCharacter* Player = Cast<AMyCharacter>(GetPawn());		//ù ���� ���� �ʱ�ȭ
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

