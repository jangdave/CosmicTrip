// Fill out your copyright notice in the Description page of Project Settings.


#include "caEnemySpawnManager.h"
#include <Kismet/GameplayStatics.h>
#include "caEnemySpawnSpot.h"
#include "CloseAttackEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Boss.h"

// Sets default values
AcaEnemySpawnManager::AcaEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		

}

// Called when the game starts or when spawned
void AcaEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AcaEnemySpawnSpot::StaticClass(), spawnList);	
	
	GetWorldTimerManager().SetTimer(timerHandlespawncaEnemy, this, &AcaEnemySpawnManager::SpawnEnemy, 1, false);
	
	//UE_LOG(LogTemp, Error, TEXT("AcaEnemySpawnManager::BeginPlay()"))
	ACloseAttackEnemy* enmey = Cast<ACloseAttackEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), caEnemyFactory));

	ABoss* boss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), bossFactory));

}

// Called every frame
void AcaEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AcaEnemySpawnManager::SpawnEnemy()
{
	EnemyCount++;

	if (limitedEnemyCount >= EnemyCount)
	{
		int randIndex = 0;		
		randIndex = FMath::RandRange(0, spawnList.Num() - 1);
		if (randIndex == prevRandIndex)
		{
			randIndex = (randIndex + 1) % spawnList.Num();
		}

		FVector loc = spawnList[randIndex]->GetActorLocation();
		FRotator rot = spawnList[randIndex]->GetActorRotation();
		GetWorld()->SpawnActor<ACloseAttackEnemy>(caEnemyFactory, loc, rot);

		int randomTime = FMath::RandRange(minTime, maxTime);
		GetWorldTimerManager().SetTimer(timerHandlespawncaEnemy, this, &AcaEnemySpawnManager::SpawnEnemy, randomTime);
		prevRandIndex = randIndex;

	}
	else
	{
		GetWorldTimerManager().ClearTimer(timerHandlespawncaEnemy);

		//Boss 스폰하는 함수를 호출해준다.
		GetWorld()->GetTimerManager().SetTimer(timerHandleSpawnBoss, this, &AcaEnemySpawnManager::SpawnBoss, 5);

	}
}

void AcaEnemySpawnManager::SpawnBoss()
{	

	int randIndex = 0;	
	randIndex = FMath::RandRange(0, spawnList.Num() - 1);
	if (randIndex == prevRandIndex)
	{
		randIndex = (randIndex + 1) % spawnList.Num();
	}

	FVector loc = spawnList[randIndex]->GetActorLocation();
	FRotator rot = spawnList[randIndex]->GetActorRotation();
	GetWorld()->SpawnActor<ABoss>(bossFactory, loc, rot);
	//prevRandIndex = randIndex;
}

