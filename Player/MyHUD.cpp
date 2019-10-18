// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager LoadAsset;

	FSoftObjectPath CrossHairAsset(TEXT("Texture2D'/Game/UI/crosshair.crosshair'"));
	Crosshair = Cast<UTexture2D>(LoadAsset.LoadSynchronous(CrossHairAsset));
}

void AMyHUD::DrawHUD()
{
	if (Crosshair)
	{
		DrawTextureSimple(Crosshair, Canvas->SizeX / 2 - Crosshair->GetSizeX() / 2, Canvas->SizeY / 2 - Crosshair->GetSizeY() / 2);
	}
}
