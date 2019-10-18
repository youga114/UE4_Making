// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Zombie.h"

#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

};
