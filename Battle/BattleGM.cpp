// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGM.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Flag.h"
#include "Flag/FlagPoint.h"
#include "Battle/BattleGS.h"

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	// 맵에서 Flag 생성 위치 가져오기(포인트를 이용하여)
	TArray<AActor*> FlagLocations;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlagPoint::StaticClass(), FlagLocations);

	// Flag 생성
	for (auto Location : FlagLocations)
	{
		FActorSpawnParameters Parameter;
		Parameter.Name = Location->Tags[0];

		Cast<AFlag>(GetWorld()->SpawnActor<AFlag>(FlagClass, Location->GetTransform(), Parameter));
	}
}
