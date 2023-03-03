// Fill out your copyright notice in the Description page of Project Settings.


#include "Refinery.h"
#include "Components/BoxComponent.h"

// Sets default values
ARefinery::ARefinery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Cube_02.SM_Cube_02'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ARefinery::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARefinery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

