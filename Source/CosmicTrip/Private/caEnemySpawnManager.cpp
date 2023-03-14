// Fill out your copyright notice in the Description page of Project Settings.


#include "caEnemySpawnManager.h"
#include <Kismet/GameplayStatics.h>
#include "caEnemySpawnSpot.h"
#include "CloseAttackEnemy.h"
#include "Components/StaticMeshComponent.h"

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
	
	//int randTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(timerHandlespawncaEnemy, this, &AcaEnemySpawnManager::SpawnEnemy, 1);
	//UE_LOG(LogTemp, Error, TEXT("AcaEnemySpawnManager::BeginPlay()"))
	auto enmey = Cast<ACloseAttackEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), caEnemyFactory));
}

// Called every frame
void AcaEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AcaEnemySpawnManager::SpawnEnemy()
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

	//UE_LOG(LogTemp, Warning, TEXT("AcaEnemySpawnManager::SpawnEnemy()"))

}

