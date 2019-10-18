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

	// �ʿ��� Flag ���� ��ġ ��������(����Ʈ�� �̿��Ͽ�)
	TArray<AActor*> FlagLocations;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlagPoint::StaticClass(), FlagLocations);

	// Flag ����
	for (auto Location : FlagLocations)
	{
		FActorSpawnParameters Parameter;
		Parameter.Name = Location->Tags[0];

		Cast<AFlag>(GetWorld()->SpawnActor<AFlag>(FlagClass, Location->GetTransform(), Parameter));
	}
}
