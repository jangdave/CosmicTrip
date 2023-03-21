// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerRobot.h"
#include "Boss.h"
#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "CosmicPlayer.h"
#include "RazerFSMComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARazerRobot::ARazerRobot()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/VTH/MESHES/VTH_SK.VTH_SK'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -45.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);

	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;

	razerFSM = CreateDefaultSubobject<URazerFSMComponent>(TEXT("razerFSM"));
}

void ARazerRobot::BeginPlay()
{
	Super::BeginPlay();

	boss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

	player = Cast<ACosmicPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACosmicPlayer::StaticClass()));
}

void ARazerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARazerRobot::StartFire()
{
	if (fireEffect != nullptr)
	{
		niagaraFireComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireEffect, GetActorLocation());
		FVector loc = razerFSM->enemis[0]->GetActorLocation();
		niagaraFireComp->SetVariableVec3(FName("AimPosition"), loc);

		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(boss);
		params.AddIgnoredActor(player);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), razerFSM->enemis[0]->GetActorLocation(), ECollisionChannel::ECC_Visibility, params);

		if (bHit)
		{
			auto target = Cast<ACloseAttackEnemy>(hitResult.GetActor());
			target->caEnemyFSM->OnTakeDamage(10.0f);
			Impact(hitResult.Location);
		}
	}
}

void ARazerRobot::Impact(FVector loc)
{
	if (impactEffect)
	{
		impactComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactEffect, loc);
	}
}
