// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGS.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Battle/BattlePC.h"
#include "Flag.h"

//void ABattleGS::BeginPlay()
//{
//	Super::BeginPlay();
//
//}
//
//void ABattleGS::ChangeFlagPercent_OnRep()
//{
//	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	if (PC)
//	{
//		for (auto AnyFlag : AllFlag)
//		{
//			if (AnyFlag == PC->Flag)
//			{
//				AFlag* Flag = Cast<AFlag>(AnyFlag);
//				if (Flag)
//				{
//					PC->SetPercent(Flag->Percent);
//				}
//			}
//		}
//	}
//}
//
//void ABattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ABattleGS, AllFlag);				// 네트워크로 정보 보내주는 포트 생성
//}
