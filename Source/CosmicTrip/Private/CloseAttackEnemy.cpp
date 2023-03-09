// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "CloseAttackEnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACloseAttackEnemy::ACloseAttackEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempcaMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempcaMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempcaMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/CosmicVR/Enemy/Animation/ABP_CloseAttackEnemyAnim.ABP_CloseAttackEnemyAnim_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}
	
	caEnemyFSM = CreateDefaultSubobject<UCloseAttackEnemyFSM>(TEXT("caEnemyFSM"));

	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
	gunMesh->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempgunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempgunMesh.Succeeded())
	{
		gunMesh->SetSkeletalMesh(tempgunMesh.Object);
		gunMesh->SetRelativeLocationAndRotation(FVector(30, 30, 120), FRotator(0, 0, 0));
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

// Called when the game starts or when spawned
void ACloseAttackEnemy::BeginPlay()
{
	Super::BeginPlay();

	caEnemyAnim = Cast<UCloseAttackEnemyAnim>(GetMesh()->GetAnimInstance());
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	

	
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


