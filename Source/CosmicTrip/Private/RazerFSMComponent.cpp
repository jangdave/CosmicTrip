// Fill out your copyright notice in the Description page of Project Settings.


#include "RazerFSMComponent.h"
#include "AIController.h"
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

	aiRazer = Cast<AAIController>(me->GetController());

	player = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

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
	aiRazer->MoveToLocation(player->GetActorLocation());
	
	float dist = me->GetDistanceTo(player);

	if (dist < 500.0f)
	{
		SetRazerState(ERazerState::PATROL);
	}
}

void URazerFSMComponent::TickPatrol()
{
	FVector newLocation = player->GetActorLocation();
	
	angleAxis+= multiplier;

	if(angleAxis >=360.0f)
	{
		angleAxis = 0;
	}

	FVector rotateValue = dimensions.RotateAngleAxis(angleAxis, axisVector);

	newLocation.X += rotateValue.X;
	newLocation.Y += rotateValue.Y;
	newLocation.Z += rotateValue.Z;

	aiRazer->MoveToLocation(newLocation);

	enemy = Cast<ACloseAttackEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ACloseAttackEnemy::StaticClass()));

	float dist = me->GetDistanceTo(enemy);

	if (dist < 1000.0f)
	{
		SetRazerState(ERazerState::ATTACK);
	}
}

void URazerFSMComponent::TickAttack()
{
	aiRazer->MoveToLocation(enemy->GetActorLocation());

	float dist = me->GetDistanceTo(enemy);

	if (dist < 300.0f)
	{
		SetRazerState(ERazerState::DAMAGE);
	}
	if (enemy == nullptr)
	{
		SetRazerState(ERazerState::PATROL);
	}
}

void URazerFSMComponent::TickDamage()
{
	if (bCheckEmitter != true)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), me->beamFactory, me->GetActorTransform());
		bCheckEmitter = true;
	}

	float dist = me->GetDistanceTo(enemy);

	if (dist > 600.0f)
	{
		SetRazerState(ERazerState::ATTACK);
		bCheckEmitter = false;
	}
	if (enemy == nullptr)
	{
		SetRazerState(ERazerState::PATROL);
		bCheckEmitter = false;
	}
}

void URazerFSMComponent::TickDie()
{

}

void URazerFSMComponent::OnDamage(int32 damage)
{

}

void URazerFSMComponent::SetRazerState(ERazerState next)
{
	razerState = next;
}

