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

	bReplicates = true;							//��Ʈ��ũ �ϰڴٰ� ����(ĳ������ ��� ����Ʈ�� ��������, ������ �츮�� ���� ������ ��� ����Ʈ�� ��������)
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AFlag::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFlag::OnEndOverlap);

	//SpawnTimerFunction();

	if (HasAuthority())
	{
		S2A_MultiChangeFlagColor(FName(*GetName()));
	}

	GetWorld()->GetTimerManager().SetTimer(PercentTimerHandle, this, &AFlag::PercentTimer, 0.1f, true);
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

	Percent = 0.0f; 
	if (HasAuthority())
	{
		ChangeFlagPercent_OnRep();
	}

	Swap<float>(TeamCount,EnemyCount);
}

void AFlag::S2A_MultiChangeFlagColor_Implementation(FName Color)
{
	if (HasAuthority())
	{
		FlagColor = Color;
		ChangeFlagColor_OnRep();
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

		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFlag::SpawnTimerFunction, 10.0f);
	}
}

void AFlag::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
	if (Player)
	{
		ABattlePC* PC = Player->GetController<ABattlePC>();
		if (PC && PC->IsLocalController())
		{
			PC->SetAndShowFlagWidget(FlagColor);
			PC->Flag = this;
		}

		if (HasAuthority())
		{
			if (FlagColor == "White")		//��� ����� ���
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
		ABattlePC* PC = Player->GetController<ABattlePC>();
		if (PC)
		{
			PC->HideFlagBoundary();
			PC->Flag = nullptr;
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

void AFlag::ChangeFlagPercent_OnRep()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if (this == PC->Flag)
		{
			PC->SetPercent(Percent);
		}
	}
}

void AFlag::ChangeFlagColor_OnRep()
{
	SetInitFlag(FlagColor);

	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->Flag == this)
	{
		PC->SetAndShowFlagWidget(FlagColor);
	}
}

void AFlag::PercentTimer()
{
	if (FlagColor == "White")
	{
		if (BlueCount > RedCount)
		{

		}
		else if (BlueCount < RedCount)
		{

		}
	}

	if (EnemyCount > TeamCount)
	{
		if (HasAuthority())
		{
			Percent += EnemyCount / TeamCount / 200;
			ChangeFlagPercent_OnRep();				//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����

			if (Percent >= 1)
			{
				FlagColor = (FlagColor == "Red") ? "Blue" : "Red";
				ChangeFlagColor_OnRep();			//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����
			}
		}
	}

	//if (FlagColor != "White")
	//{
	//	if (EnemyCount > TeamCount)
	//	{
	//		if (HasAuthority())
	//		{
	//			Percent += EnemyCount / TeamCount / 200;
	//			ChangeFlagPercent_OnRep();				//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����

	//			if (Percent >= 1)
	//			{
	//				FlagColor = (FlagColor == "Red") ? "Blue" : "Red";
	//				ChangeFlagColor_OnRep();			//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	if (TeamCount > EnemyCount)				//Blue�� �� ������
	//	{
	//		if (HasAuthority())
	//		{
	//			Percent += EnemyCount / TeamCount / 200;
	//			ChangeFlagPercent_OnRep();				//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����

	//			if (Percent >= 1)
	//			{
	//				FlagColor = "Blue";
	//				ChangeFlagColor_OnRep();			//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����
	//			}
	//		}
	//	}
	//	else if (TeamCount < EnemyCount)		//Red�� �� ������
	//	{
	//		if (HasAuthority())
	//		{
	//			Percent += TeamCount / EnemyCount / 200;
	//			ChangeFlagPercent_OnRep();				//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����

	//			if (Percent >= 1)
	//			{
	//				FlagColor = "Red";
	//				ChangeFlagColor_OnRep();			//������ ��� Replicate�Լ��� ������ �ȵǹǷ� ���� ����
	//			}
	//		}
	//	}
	//}
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlag, Percent);				// ��Ʈ��ũ�� ���� �����ִ� ��Ʈ ����
	DOREPLIFETIME(AFlag, FlagColor);				// ��Ʈ��ũ�� ���� �����ִ� ��Ʈ ����
}