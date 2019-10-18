// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE3_API UAnimNotify_Attack : public UAnimNotify
{
	GENERATED_BODY()
public:
	int a;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
