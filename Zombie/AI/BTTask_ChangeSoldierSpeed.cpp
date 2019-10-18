// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSoldierSpeed.h"
#include "Zombie/AI/SoldierAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UBTTask_ChangeSoldierSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyCharacter* Soldier = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Soldier)
	{
		return EBTNodeResult::Failed;
	}

	if (CurrentSpeed == ESoldierSpeed::Walk)
	{
		Soldier->GetCharacterMovement()->MaxWalkSpeed = Soldier->JogSpeed;
		Soldier->bIsSprint = false;
	}
	else if (CurrentSpeed == ESoldierSpeed::Run)
	{
		Soldier->GetCharacterMovement()->MaxWalkSpeed = Soldier->SprintSpeed;
		Soldier->bIsSprint = true;
	}
	else
	{
		Soldier->GetCharacterMovement()->MaxWalkSpeed = Soldier->StopSpeed;
		Soldier->bIsSprint = false;
	}

	return EBTNodeResult::Succeeded;
}
