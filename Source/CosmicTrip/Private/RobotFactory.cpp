// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotFactory.h"
#include "RazerRobot.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARobotFactory::ARobotFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Cube_01.SM_Cube_01'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ARobotFactory::BeginPlay()
{
	Super::BeginPlay();

	razer = Cast<ARazerRobot>(UGameplayStatics::GetActorOfClass(GetWorld(), razerFactory));

	// 임시 스폰
	FTimerHandle timer;
	//GetWorld()->GetTimerManager().SetTimer(timer, this, &ARobotFactory::SpawnRazerRobot, 3, false);
}

// Called every frame
void ARobotFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARobotFactory::SpawnRazerRobot()
{
	GetWorld()->SpawnActor<ARazerRobot>(razerFactory, GetActorLocation() + GetActorForwardVector() * 200.0f, GetActorRotation());
}

