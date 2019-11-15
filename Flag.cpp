// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Player/MyCharacter.h"
#include "Battle/BattlePC.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/BattleGS.h"
#include "UnrealNetwork.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Capsule);

	Boundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boundary"));
	Boundary->SetupAttachment(Capsule);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Body);

	bReplicates = true;							//네트워크 하겠다고 선언(캐릭터의 경우 디폴트로 켜져있음, 하지만 우리가 만든 액터의 경우 디폴트로 꺼져있음)
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AFlag::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFlag::OnEndOverlap);

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(PercentTimerHandle, this, &AFlag::PercentTimer, 0.1f, true);

		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFlag::SpawnTimerFunction, 20.0f, true);
	}
}

void AFlag::SetInitFlag(FName FlagColor)
{
	if (FlagColor=="Red")
	{
		Body->SetMaterial(0, RedBody);
		Boundary->SetMaterial(0, RedBoundary);
	}
	else if (FlagColor=="Blue")
	{
		Body->SetMaterial(0, BlueBody);
		Boundary->SetMaterial(0, BlueBoundary);
	}
	else
	{
		Body->SetMaterial(0, WhiteBody);
		Boundary->SetMaterial(0, WhiteBoundary);
	}

	Swap<float>(TeamCount, EnemyCount);

	Percent = 0.0f; 
	if (HasAuthority())
	{
		ChangeFlagPercent_OnRep();
	}
}

void AFlag::SpawnTimerFunction()
{
	if (HasAuthority())
	{
		if (FlagColor=="Red")
		{
			GetWorld()->SpawnActor<AActor>(RedClass, Arrow->K2_GetComponentToWorld());
		}
		else if (FlagColor == "Blue")
		{
			GetWorld()->SpawnActor<AActor>(BlueClass, Arrow->K2_GetComponentToWorld());
		}
	}
}

void AFlag::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
	if (Player)
	{
		Player->Flag = this;

		ABattlePC* PC = Player->GetController<ABattlePC>();
		if (PC && PC->IsLocalController())
		{
			PC->SetOccupationText("Occupied");
			PC->SetPercent(Percent);
			PC->SetAndShowFlagWidget(FlagColor);
		}

		if (HasAuthority())
		{
			if (FlagColor == "White")		//흰색 깃발의 경우
			{
				if (Player->ActorHasTag("Blue"))
				{
					BlueCount++;
				}
				else
				{
					RedCount++;
				}
			}
			else
			{
				if (Player->ActorHasTag(FlagColor))
				{
					TeamCount++;
				}
				else
				{
					EnemyCount++;
				}
			}
		}
	}
}

void AFlag::OnEndOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
	if (Player)
	{
		Player->Flag = nullptr;

		ABattlePC* PC = Player->GetController<ABattlePC>();
		if (PC)
		{
			PC->HideFlagBoundary();
		}

		if (HasAuthority())
		{
			if (FlagColor == "White")
			{
				if (Player->ActorHasTag("Blue"))
				{
					BlueCount--;
				}
				else
				{
					RedCount--;
				}
			}
			else
			{
				if (Player->ActorHasTag(FlagColor))
				{
					TeamCount--;
				}
				else
				{
					EnemyCount--;
				}
			}
		}
	}
}

void AFlag::ChangeGaugeColor_OnRep()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(PC->GetPawn());
		if (Player && (this == Player->Flag))
		{
			PC->SetGaugeColor(WillFlagColor);
		}
	}
}

void AFlag::ChangeFlagPercent_OnRep()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(PC->GetPawn());
		if (Player && (this == Player->Flag))
		{
			PC->SetOccupationText("Occuping");
			PC->SetPercent(Percent);
		}
	}
}

void AFlag::ChangeFlagColor_OnRep()
{
	SetInitFlag(FlagColor);

	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(PC->GetPawn());
		if (Player && (Player->Flag == this))
		{
			PC->SetOccupationText("Occupied");
			PC->SetAndShowFlagWidget(FlagColor);
		}
	}
}

void AFlag::PercentTimer()
{
	if (FlagColor == "White")
	{
		if (HasAuthority())
		{
			if (BlueCount > RedCount)
			{
				EnemyCount = BlueCount;
				TeamCount = RedCount;

				if (WillFlagColor != "Blue")
				{
					WillFlagColor = "Blue";
					ChangeGaugeColor_OnRep();			//서버의 경우 Replicate함수가 실행이 안되므로 직접 실행
				}
			}
			else if (BlueCount < RedCount)
			{
				EnemyCount = RedCount;
				TeamCount = BlueCount;
				if (WillFlagColor != "Red")
				{
					WillFlagColor = "Red";
					ChangeGaugeColor_OnRep();			//서버의 경우 Replicate함수가 실행이 안되므로 직접 실행
				}
			}
			else
			{
				EnemyCount = RedCount;
				TeamCount = BlueCount;
			}
		}
	}

	if (EnemyCount > TeamCount)
	{
		if (HasAuthority())
		{

			Percent += EnemyCount / TeamCount / 200;
			ChangeFlagPercent_OnRep();				//서버의 경우 Replicate함수가 실행이 안되므로 직접 실행

			if (Percent >= 1)
			{
				if (FlagColor == "White")
				{
					FlagColor = WillFlagColor;
				}
				else
				{
					FlagColor = (FlagColor == "Red") ? "Blue" : "Red";
				}
				ChangeFlagColor_OnRep();			//서버의 경우 Replicate함수가 실행이 안되므로 직접 실행

				TArray<AActor*> OutActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlag::StaticClass(), OutActors);

				bool enemyFlag = false;
				for (auto flag : OutActors)
				{
					AFlag* anyFlag = Cast<AFlag>(flag);
					if (anyFlag && anyFlag->FlagColor != FlagColor)
					{
						enemyFlag = true;
						break;
					}
				}
				if (!enemyFlag)
				{
					GetWorld()->ServerTravel(TEXT("Lobby"));
					//게임 끝 로비로
				}
			}
		}
	}
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlag, Percent);				// 네트워크로 정보 보내주는 포트 생성
	DOREPLIFETIME(AFlag, FlagColor);				// 네트워크로 정보 보내주는 포트 생성
	DOREPLIFETIME(AFlag, WillFlagColor);				// 네트워크로 정보 보내주는 포트 생성
}