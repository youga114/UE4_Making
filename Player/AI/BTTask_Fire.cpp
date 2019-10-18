// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"
#include "Player/MyCharacter.h"
#include "Zombie/AI/SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASoldierAIController* AIC = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		AMyCharacter* Enemy = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
		if (Enemy)
		{
			AMyCharacter* Soldier = Cast<AMyCharacter>(AIC->GetPawn());
			if (Soldier)
			{
				Soldier->SetActorRotation(FRotator(0, (Enemy->GetActorLocation() - Soldier->GetActorLocation()).Rotation().Yaw, 0));
				Soldier->SetAimRotator(Enemy->GetActorLocation() - Soldier->GetActorLocation());
				Soldier->Ironsight();
				Soldier->StartFire();
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
