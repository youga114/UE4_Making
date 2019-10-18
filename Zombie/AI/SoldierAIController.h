// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Player/MyCharacter.h"

#include "SoldierAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASoldierAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BBComponent;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void SetCurrentState(ESoldierState NewState);

	void SetTargetPlayer(AActor* Player);
	void SetTargetFlag(AActor * Flag);
};
