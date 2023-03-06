// Fill out your copyright notice in the Description page of Project Settings.


#include "caEnemySpawnSpot.h"

// Sets default values
AcaEnemySpawnSpot::AcaEnemySpawnSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawnSpotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	spawnSpotMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSpawnSpotMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
	if (tempSpawnSpotMesh.Succeeded())
	{
		spawnSpotMesh->SetStaticMesh(tempSpawnSpotMesh.Object);
		spawnSpotMesh->SetRelativeScale3D(FVector(0.8f));
		spawnSpotMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}

// Called when the game starts or when spawned
void AcaEnemySpawnSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcaEnemySpawnSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

