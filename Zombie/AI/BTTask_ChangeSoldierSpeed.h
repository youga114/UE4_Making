// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChangeSoldierSpeed.generated.h"

UENUM(BlueprintType)
enum class ESoldierSpeed : uint8
{
	Walk = 0 UMETA(Display = "Walk"),
	Run = 1 UMETA(Display = "Run"),
	Stop = 2 UMETA(Display = "Stop"),
};

/**
 * 
 */
UCLASS()
class PRACTICE3_API UBTTask_ChangeSoldierSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoldierSpeed CurrentSpeed;
};
