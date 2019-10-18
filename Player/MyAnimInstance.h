// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsCrouched : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsIronsight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireAlpha = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimPitch;
};
