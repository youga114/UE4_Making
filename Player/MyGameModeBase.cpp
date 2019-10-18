// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Zombie.h"
#include "Engine/World.h"
#include "Player/MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	//DefaultPawnClass = AMyCharacter::StaticClass();
}

int AMyGameModeBase::GetTargetCount()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Red"), OutActors);
	return OutActors.Num();
}

void AMyGameModeBase::CheckTargetCount()
{
	//if (GetTargetCount() == 0)
	//{
	//	if (GetWorld()->GetMapName() == "UEDPIE_0_Stage1")
	//	{
	//		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage2"));
	//		return;
	//	}
	//	else if (GetWorld()->GetMapName() == "UEDPIE_0_Stage2")
	//	{
	//		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage3"));
	//		return;
	//	}
	//}
}
