// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 
	PrimaryActorTick.bCanEverTick = true;

	spherComp = CreateDefaultSubobject<USphereComponent>(TEXT("spherComp"));
	SetRootComponent(spherComp);
	spherComp->SetSphereRadius(13);
	spherComp->SetCollisionProfileName(TEXT("BlockAll"));


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.25f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->SetUpdatedComponent(spherComp);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.5f;

}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	//SetLifeSpasn(2);
	//LifeSpanExpired();

	FTimerHandle dieTimerHandle;
	//GetWorldTimerManager().SetTimer(dieTimerHandle, this, &ABulletActor::OnDie, 0.1f);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletActor::OnDie()
{
	Destroy();
}

