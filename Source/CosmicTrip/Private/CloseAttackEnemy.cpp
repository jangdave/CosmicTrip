// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemy.h"

// Sets default values
ACloseAttackEnemy::ACloseAttackEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	closeAttackEnemy = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("closeAttackEnemy"));
	closeAttackEnemy->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempcaMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempcaMesh.Succeeded())
	{
		closeAttackEnemy->SetSkeletalMesh(tempcaMesh.Object);
		closeAttackEnemy->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	

}

// Called when the game starts or when spawned
void ACloseAttackEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloseAttackEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACloseAttackEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ACloseAttackEnemy::Move()
{

}


