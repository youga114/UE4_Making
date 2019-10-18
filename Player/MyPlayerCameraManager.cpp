// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCameraManager.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

void AMyPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
	if (PC)
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(PC->GetPawn());
		if (Pawn)
		{
			float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;

			SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 10.0f));

			FVector TargetPos = Pawn->bIsCrouched ? Pawn->CrouchedSpringArmPosition : Pawn->NormalSpringArmPosition;
			//Pawn->SetSpringArmPosition(FMath::VInterpTo(Pawn->GetSpringArmPosition(), TargetPos, DeltaTime, 10.0f));
		}
	}
}
