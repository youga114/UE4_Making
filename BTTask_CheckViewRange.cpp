// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckViewRange.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Zombie/AI/SoldierAIController.h"
#include "Player/MyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckViewRange::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASoldierAIController* AIC = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		AMyCharacter* Soldier = Cast<AMyCharacter>(AIC->GetPawn());
		if (Soldier)
		{
			AMyCharacter* Player = Cast<AMyCharacter>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
			if (Player)
			{
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
				ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
				ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

				TArray<AActor*> IgnoreActors;
				IgnoreActors.Add(Soldier);

				FHitResult OutHit;

				bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
					GetWorld(),
					Soldier->Weapon->GetSocketLocation(TEXT("Muzzle")),
					Player->Weapon->GetSocketLocation(TEXT("Muzzle")),
					ObjectType,
					false,
					IgnoreActors,
					EDrawDebugTrace::None,
					OutHit,
					true,
					FLinearColor::Red,
					FLinearColor::Green,
					5.0f);
				if (bResult)
				{
					if (!( (OutHit.GetActor()->ActorHasTag(TEXT("Red")) && Soldier->ActorHasTag(TEXT("Blue")) ) || ( OutHit.GetActor()->ActorHasTag(TEXT("Blue")) && Soldier->ActorHasTag(TEXT("Red"))) ) )
					{
						Soldier->StopFire();
						Soldier->UnIronsight();

						OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)TargetState);
						Soldier->CurrentState = TargetState;
					}
				}
			}
		}
	}


	return EBTNodeResult::Succeeded;
}
