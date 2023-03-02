// Fill out your copyright notice in the Description page of Project Settings.


#include "Money.h"
#include "Components/SphereComponent.h"

// Sets default values
AMoney::AMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("boxComp"));
	SetRootComponent(sphereComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(sphereComp);
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

