// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerFSMComponent.h"
#include "CloseAttackEnemy.h"
#include "CosmicPlayer.h"
#include "RazerRobot.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URazerFSMComponent::URazerFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URazerFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ARazerRobot>(GetOwner());

	player = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	enemy = Cast<ACloseAttackEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ACloseAttackEnemy::StaticClass()));

	SetRazerState(ERazerState::IDLE);
}


// Called every frame
void URazerFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (razerState)
	{
	case ERazerState::IDLE:
		TickIdle();
		break;
	case ERazerState::MOVE:
		TickMove();
		break;
	case ERazerState::PATROL:
		TickPatrol();
		break;
	case ERazerState::ATTACK:
		TickAttack();
		break;
	case ERazerState::DAMAGE:
		TickDamage();
		break;
	case ERazerState::DIE:
		TickDie();
		break;
	}
}

void URazerFSMComponent::TickIdle()
{
	if (player)
	{
		SetRazerState(ERazerState::MOVE);
	}
}

void URazerFSMComponent::TickMove()
{
	FVector dir = player->GetActorLocation() - me->GetActorLocation();

	me->AddMovementInput(dir);
	//me->SetActorLocation(me->GetActorLocation() + dir.GetSafeNormal() * 100.0f * GetWorld()->DeltaTimeSeconds);

	//float targetDist = player->GetDistanceTo(me);

	//if (targetDist < 100.0f)
	//{
		//SetRazerState(ERazerState::PATROL);
	//}
}

void URazerFSMComponent::TickPatrol()
{
	//FVector forDir = me->GetActorForwardVector();
	//FVector inDir = 

}

void URazerFSMComponent::TickAttack()
{

}

void URazerFSMComponent::TickDamage()
{

}

void URazerFSMComponent::TickDie()
{

}

void URazerFSMComponent::SetRazerState(ERazerState next)
{
	razerState = next;
}

