// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPlayerLocation.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Flag.h"

EBTNodeResult::Type UBTTask_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMyCharacter* Player = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (Player)
	{
		if (Player->CurrentHP > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Target")), Player->GetActorLocation());
			return EBTNodeResult::Succeeded;
		}
	}

	AFlag* Flag = Cast<AFlag>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Flag"))));
	if (Flag)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Target")), Flag->GetActorLocation() + FVector(25.0, 25.0, 0.0));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}