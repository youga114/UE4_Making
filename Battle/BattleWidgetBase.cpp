// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidgetBase.h"
#include "Components/ProgressBar.h"

void UBattleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UBattleWidgetBase::SetHp(float Percent)
{
	HPBar->SetPercent(Percent);
}
