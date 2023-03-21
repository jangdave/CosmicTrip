// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "BossFSM.h"
#include "CloseAttackEnemyAnim.h"
#include "CosmicPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/StaticMeshComponent.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempcaMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempcaMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempcaMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.6f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/CosmicVR/BossEnemy/Animation/ABP_BossFSM.ABP_BossFSM_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	bossFSM = CreateDefaultSubobject<UBossFSM>(TEXT("BossFSM"));

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMesh"));
	weaponMesh->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));
	

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempweaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/CosmicVR/Assets/Enemy/BossLightSaber/source/SaberToTexture.SaberToTexture'"));
	if (tempweaponMesh.Succeeded())
	{
		weaponMesh->SetStaticMesh(tempweaponMesh.Object);
		weaponMesh->SetRelativeLocationAndRotation(FVector(3.7f, 7, 7.78f), FRotator(0, -90, -100));
		weaponMesh->SetRelativeScale3D(FVector(0.15f));
	}

	weaponBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("weaponBox"));
	weaponBox->SetupAttachment(weaponMesh);
	weaponBox->SetRelativeLocationAndRotation(FVector(0, 299, 100), FRotator(0, 0, 90));
	weaponBox->SetCapsuleSize(110, 700);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//충돌 프리셋
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CloseAttackEnemyPreset"));
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	weaponBox->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnHitBossEvent);
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto GI = UGameplayStatics::GetGameInstance(GetWorld());
	USkeletalMeshComponent* TempMesh = GetMesh();
	if (GI == nullptr)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = bossFSM->walkSpeed;
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//플레이어의 체력을 깎을 것이다
void ABoss::OnHitBossEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACosmicPlayer* player = Cast<ACosmicPlayer>(OtherActor);
	if (player != nullptr)
	{
		player->OnPlayerDamage(10);
		UE_LOG(LogTemp, Warning, TEXT("UBossFSM::OnHitEvent() Subtract Damage"))
	}
}