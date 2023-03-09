// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerRobot.h"
#include "RazerFSMComponent.h"

ARazerRobot::ARazerRobot()
{
	PrimaryActorTick.bCanEverTick = true;

	razerFSM = CreateDefaultSubobject<URazerFSMComponent>(TEXT("razerFSM"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARazerRobot::BeginPlay()
{
	Super::BeginPlay();

}

void ARazerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
