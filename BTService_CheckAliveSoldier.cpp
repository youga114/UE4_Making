// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAliveSoldier.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie/AI/SoldierAIController.h"

void UBTService_CheckAliveSoldier::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASoldierAIController* AIC = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
		if (Player)
		{
			if (Player->CurrentHP <= 0)
			{
				AMyCharacter* Soldier = Cast<AMyCharacter>(AIC->GetPawn());
				if (Soldier && Soldier->CurrentState != ESoldierState::Dead)
				{
					Soldier->StopFire();
					Soldier->UnIronsight();
					Soldier->CurrentState = ESoldierState::Normal;
					AIC->SetCurrentState(Soldier->CurrentState);
				}
			}
		}
		//AMyCharacter* Target = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Target"))));
		//if (!Target)
		//{
		//	AIC->SetTargetPlayer(AIC->GetPawn());
		//}
	}
}
