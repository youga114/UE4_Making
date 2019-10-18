// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	virtual void UpdateCamera(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BluePrintReadOnly)
	float NormalFOV = 90.0f;

	UPROPERTY(EditAnywhere, BluePrintReadOnly)
	float IronsightFOV = 50.0f;
};
