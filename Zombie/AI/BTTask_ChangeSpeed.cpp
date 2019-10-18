// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Zombie/AI/ZombieAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombie* Zombie = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}

	if (CurrentSpeed == EZombieSpeed::Walk)
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
	}
	else
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
	}

	return EBTNodeResult::Succeeded;
}
