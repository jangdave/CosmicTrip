// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"

// Sets default values
ACloseAttackEnemy::ACloseAttackEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempcaMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempcaMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempcaMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	
	caEnemyFSM = CreateDefaultSubobject<UCloseAttackEnemyFSM>(TEXT("caEnemyFSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> gunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (gunMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(gunMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(30, 0, 40), FRotator(0, -90, 0));
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
