// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetFlagLocation.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Zombie/AI/SoldierAIController.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_GetFlagLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMyCharacter* AI = Cast< AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AI)
	{
		AI->FindFlag();
	}

	return EBTNodeResult::Succeeded;
}
