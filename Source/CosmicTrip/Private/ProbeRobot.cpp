// Fill out your copyright notice in the Description page of Project Settings.


#include "ProbeRobot.h"
#include "ProbeFSMComponent.h"

AProbeRobot::AProbeRobot()
{
	PrimaryActorTick.bCanEverTick = true;

	probeFSM = CreateDefaultSubobject<UProbeFSMComponent>(TEXT("probeFSM"));
}

void AProbeRobot::BeginPlay()
{
	Super::BeginPlay();
}

void AProbeRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
