// Fill out your copyright notice in the Description page of Project Settings.


#include "ProbeFSMComponent.h"
#include "Money.h"
#include "ProbeRobot.h"
#include "Refinery.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProbeFSMComponent::UProbeFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProbeFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	me = Cast<AProbeRobot>(GetOwner());

	

	home = Cast<ARefinery>(UGameplayStatics::GetActorOfClass(GetWorld(), ARefinery::StaticClass()));

	probeState = EProbeState::IDLE;
}


// Called every frame
void UProbeFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	switch (probeState)
	{
	case EProbeState::IDLE:
		TickIdle();
		break;
	case EProbeState::MOVEMONEY:
		TickMoveMoney();
		break;
	case EProbeState::COLLECT:
		TickCollect();
		break;
	case EProbeState::MOVEHOME:
		TickMoveHome();
		break;
	case EProbeState::DIE:
		TickDie();
		break;
	}
}

void UProbeFSMComponent::TickIdle()
{
	target = Cast<AMoney>(UGameplayStatics::GetActorOfClass(GetWorld(), AMoney::StaticClass()));

	if(target != nullptr)
	{
		SetProbeState(EProbeState::MOVEMONEY);
	}
}

void UProbeFSMComponent::TickMoveMoney()
{
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	float targetDist = target->GetDistanceTo(me);

	//if(targetDist < 50.0f)
	//{
		//SetProbeState(EProbeState::COLLECT);
	//}
}

void UProbeFSMComponent::TickCollect()
{
	//SetProbeState(EProbeState::MOVEHOME);
}

void UProbeFSMComponent::TickMoveHome()
{
	//FVector dir = home->GetActorLocation() - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());

	//float targetDist = home->GetDistanceTo(me);

	//if(targetDist < 50.0f)
	//{
	//	SetProbeState(EProbeState::IDLE);
	//}
}

void UProbeFSMComponent::TickDie()
{
}

void UProbeFSMComponent::SetProbeState(EProbeState next)
{
	probeState = next;
}

