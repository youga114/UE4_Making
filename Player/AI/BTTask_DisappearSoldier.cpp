// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DisappearSoldier.h"
#include "Zombie/AI/SoldierAIController.h"
#include "Player/MyPlayerController.h"
#include "Player/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

EBTNodeResult::Type UBTTask_DisappearSoldier::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASoldierAIController* AIC = Cast<ASoldierAIController>(OwnerComp.GetOwner());

	if (AIC)
	{
		AIC->GetPawn()->Destroy();

		AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->CheckTargetCount();
		}

		return EBTNodeResult::Succeeded;
	}

	AMyPlayerController* PC = Cast<AMyPlayerController>(OwnerComp.GetOwner());
	if (AIC)
	{
		AIC->GetPawn()->Destroy();
	}

	return EBTNodeResult::Succeeded;
}
