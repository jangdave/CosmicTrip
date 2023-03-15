// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerRobot.h"
#include "RazerFSMComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ARazerRobot::ARazerRobot()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/VTH/MESHES/VTH_SK.VTH_SK'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -45.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(80.0f);

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
