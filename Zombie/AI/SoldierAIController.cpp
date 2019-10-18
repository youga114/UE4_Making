// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

ASoldierAIController::ASoldierAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void ASoldierAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AMyCharacter* Soldier = Cast<AMyCharacter>(InPawn);
	if (Soldier && BTComponent && BBComponent)
	{
		BBComponent->InitializeBlackboard(*(Soldier->SoldierAI->BlackboardAsset));
		BTComponent->StartTree(*(Soldier->SoldierAI));

		SetCurrentState(ESoldierState::Normal);
	}
}

void ASoldierAIController::OnUnPossess()
{
	BTComponent->StopTree();

	Super::OnUnPossess();
}

void ASoldierAIController::SetCurrentState(ESoldierState NewState)
{
	BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
}

void ASoldierAIController::SetTargetPlayer(AActor * Player)
{
	BBComponent->SetValueAsObject(FName(TEXT("Player")), Player);
}

void ASoldierAIController::SetTargetFlag(AActor * Flag)
{
	BBComponent->SetValueAsObject(FName(TEXT("Flag")), Flag);
}
