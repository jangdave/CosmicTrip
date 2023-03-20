// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "Boss.h"
#include "BossFSM.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	movementComp->bShouldBounce = false;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle dieTimerHandle;

	GetWorldTimerManager().SetTimer(dieTimerHandle, this, &ABulletActor::OnDie, 2.0f);

	spherComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::BulletPower);
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

void ABulletActor::BulletPower(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACloseAttackEnemy* enemy = Cast<ACloseAttackEnemy>(OtherActor);
	ABoss* boss = Cast<ABoss>(OtherActor);

	if (enemy != nullptr)
	{
		enemy->caEnemyFSM->OnTakeDamage(10);
		Destroy();
	}

	if (boss != nullptr)
	{
		boss->bossFSM->OnDamageProcess(30);
		Destroy();
	}
}

