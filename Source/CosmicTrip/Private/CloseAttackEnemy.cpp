// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "CloseAttackEnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


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

	gunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMesh"));
	gunMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempgunMesh(TEXT("/Script/Engine.StaticMesh'/Game/CosmicVR/Assets/Enemy/EnemyHammer/scene.scene'"));
	if (tempgunMesh.Succeeded())
	{
		gunMesh->SetStaticMesh(tempgunMesh.Object);
		gunMesh->SetRelativeLocationAndRotation(FVector(-8, 6, -20), FRotator(0, 180, 0));
		gunMesh->SetRelativeScale3D(FVector(0.2f));
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CloseAttackEnemyPreset"));
	
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


