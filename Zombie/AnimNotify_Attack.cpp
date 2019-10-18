// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"
#include "Zombie/Zombie.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->Attack();
	}
}
