// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Disappear.h"
#include "Zombie/Zombie.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Player/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

EBTNodeResult::Type UBTTask_Disappear::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}

	Zombie->Destroy();

	//AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (GM)
	//{
	//	GM->CheckTargetCount();
	//}

	return EBTNodeResult::Succeeded;
}
