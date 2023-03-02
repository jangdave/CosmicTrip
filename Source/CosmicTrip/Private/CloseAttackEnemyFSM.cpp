// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "CosmicPlayer.h"
#include "CloseAttackEnemy.h"

// Sets default values for this component's properties
UCloseAttackEnemyFSM::UCloseAttackEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UCloseAttackEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	state = EEnemyState::IDLE;
	me = Cast<ACloseAttackEnemy>(GetOwner());
	hp = maxHP;
	
}


// Called every frame
void UCloseAttackEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::IDLE:
		TickIdle();
		break;
	case EEnemyState::MOVE:
		TickMove();
		break;
	case EEnemyState::ATTACK:
		TickAttack();
		break;
	case EEnemyState::DAMAGE:
		TickDamage();
		break;
	case EEnemyState::DIE:
		TickDie();
		break;	
	}
	
}

void UCloseAttackEnemyFSM::TickIdle()
{
	mainTarget = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!mainTarget) return;
	state = EEnemyState::MOVE;

}

void UCloseAttackEnemyFSM::TickMove()
{
	//플레이어를 향해 달려온다
	FVector targetDir = mainTarget->GetActorLocation() - me->GetActorLocation();
	me->AddMovementInput(targetDir.GetSafeNormal());

	float targetDist = mainTarget->GetDistanceTo(me);

	//공격 범위에 들어오면 공격 상태로 전환
	if (targetDist < attackRange)
	{
		state = EEnemyState::ATTACK;
	}
}


void UCloseAttackEnemyFSM::TickAttack()
{
	//플레이어를 향해 총알을 날린다
	//Shoot();
	
	
}

void UCloseAttackEnemyFSM::TickDamage()
{
	//플레이어의 공격을 받았다면 공격당한 애니메이션 재생


// 	hp가 깎인다
// 		if (hp = 0)
// 		{
// 			//hp가 다 깎이면 죽음 상태로 전환한다
// 			state = EEnemyState::DIE;
// 		}
}

void UCloseAttackEnemyFSM::TickDie()
{
	//죽는 애니메이션을 재생시킨다

}

void UCloseAttackEnemyFSM::OnTakeDamage()
{
}