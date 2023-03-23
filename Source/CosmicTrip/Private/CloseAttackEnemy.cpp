// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "CloseAttackEnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CosmicPlayer.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraComponent.h"


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
		gunMesh->SetCollisionProfileName(TEXT("AxePreset"));
	}

	axeBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("axeBox"));
	axeBox->SetupAttachment(gunMesh);
	axeBox->SetRelativeLocationAndRotation(FVector(0, -11, 350), FRotator(0, 0, 0));
	axeBox->SetCapsuleSize(130, 300);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CloseAttackEnemyPreset"));

	/*Sound*/
	axeAttackSound = CreateDefaultSubobject<USoundBase>(TEXT("axeAttackSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> attackSound(TEXT("/Script/Engine.SoundWave'/Game/CosmicVR/Assets/Sound/AxeThrowSwing.AxeThrowSwing'"));
	
	if (attackSound.Succeeded())
	{
		axeAttackSound = attackSound.Object;
	}
	
	
}

// Called when the game starts or when spawned
void ACloseAttackEnemy::BeginPlay()
{
	Super::BeginPlay();

	caEnemyAnim = Cast<UCloseAttackEnemyAnim>(GetMesh()->GetAnimInstance());
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	axeBox->SetGenerateOverlapEvents(true);
	axeBox->OnComponentBeginOverlap.AddDynamic(this, &ACloseAttackEnemy::OnOverlapEnemyEvent);
	axeBox->OnComponentEndOverlap.AddDynamic(this, &ACloseAttackEnemy::EndOverlapEnemyEvent);

	SpawnEffect();
	
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

//플레이어의 체력을 깎을 것이다
void ACloseAttackEnemy::OnOverlapEnemyEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("UUUUUUUUUUUCloseAttackEnemyFSM::OnHitEvent Subtract Damage"))
	auto player = Cast<ACosmicPlayer>(OtherActor);
	if (player != nullptr && bISHit != false)
	{
		player->OnPlayerDamage(4);
		UE_LOG(LogTemp, Warning, TEXT("UUUUUUUUUUUCloseAttackEnemyFSM::OnHitEvent Subtract Damage"))
	}
}

void ACloseAttackEnemy::EndOverlapEnemyEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("EndOverlapEndOverlapEndOverlapEndOverlapEndOverlapEndOverlapEndOverlap"))
	bISHit = false;
}

void ACloseAttackEnemy::SpawnEffect()
{
	if (spawnEffect != nullptr)
	{
		spawnComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), spawnEffect, GetActorLocation() - FVector(0, 0, 90));
		
		FTimerHandle spawnTimer;
		GetWorld()->GetTimerManager().SetTimer(spawnTimer, FTimerDelegate::CreateLambda(
			[this]()->void
			{
				spawnComp->DestroyInstance();
			}

		), 4, false);
	}

}


