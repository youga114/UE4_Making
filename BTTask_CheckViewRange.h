// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Player/MyCharacter.h"

#include "BTTask_CheckViewRange.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API UBTTask_CheckViewRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoldierState TargetState;
};
