// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerRobot.h"
#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "RazerFSMComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), razerFSM->enemis[0]->GetActorLocation(), ECollisionChannel::ECC_Visibility, params);

		if (bHit)
		{
			auto target = Cast<ACloseAttackEnemy>(hitResult.GetActor());
			target->caEnemyFSM->OnTakeDamage(10.0f);
		}
	}
}