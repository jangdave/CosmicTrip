// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "CosmicPlayer.h"
#include "CloseAttackEnemy.h"
#include <NavigationSystem.h>
#include "AIController.h"
#include "CloseAttackEnemyAnim.h"
#include "RazerRobot.h"

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
	ai = Cast<AAIController>(me->GetController());
	hp = maxHP;
	
	//랜덤한위치에 있을 것	
	UpdateRandomLocation(randLocationRadius, randomLocation);

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
	case EEnemyState::MOVETOROBOT:
		TickMoveToRobot();
		break;
	case EEnemyState::ATTACK:
		TickAttack();
		break;	
	case EEnemyState::ATTACKROBOT:
		TickAttackRobot();
		break;
	case EEnemyState::DAMAGE:
		TickDamage();
		break;
	case EEnemyState::DIE:
		TickDie();
		break;	
	}	
}

void UCloseAttackEnemyFSM::SetState(EEnemyState next)
{
	state = next;
	me->caEnemyAnim->state = next;
	currentTime = 0;
}

void UCloseAttackEnemyFSM::TickIdle()
{	
	mainTarget = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!mainTarget) return;
	

	SetState(EEnemyState::MOVE);
}

void UCloseAttackEnemyFSM::TickMove()
{	
	//플레이어의 방향, 위치 찾는다
	//플레이어가 나와 감지거리 안에 있다면 플레이어를 향해 이동한다
	
	FVector targetDir = mainTarget->GetActorLocation() - me->GetActorLocation();
	targetDist = mainTarget->GetDistanceTo(me);
	me->caEnemyAnim->bChooseWalk = true;
	if (targetDist <= trackingRange)
	{
		ai->MoveToLocation(mainTarget->GetActorLocation());
		wantedLocation = mainTarget->GetActorLocation();
	}

	//플레이어를 공격할 범위 안에 들어왔다면 플레이어 공격 상태로 전환
	if (attackRange >= targetDist)
	{
		SetState(EEnemyState::ATTACK);
	}

	//로봇을 찾는다
	//로봇이 나와 감지범위 안에 있으면 로봇으로 향하는 state를 만든다
	razerTarget = Cast<ARazerRobot>(UGameplayStatics::GetActorOfClass(GetWorld(), ARazerRobot::StaticClass()));
	
	if (trackingRange <= trackingRobotRange)
	{
		SetState(EEnemyState::MOVETOROBOT);
	}

}

//로봇이 나와 감지거리 안에 있다면 로봇을 향해 이동한다
void UCloseAttackEnemyFSM::TickMoveToRobot()
{
	//로봇의 방향과 위치를 찾는다
	FVector robotDir = razerTarget->GetActorLocation() - me->GetActorLocation();
	razerTargetDist = razerTarget->GetDistanceTo(me);

	//로봇을 향해 이동한다
	ai->MoveToLocation(razerTarget->GetActorLocation());

	//로봇을 공격할 범위 안에 들어왔다면 로봇 공격 상태로 전환
	if (razerTargetDist <= attackRange)
	{
		SetState(EEnemyState::ATTACKROBOT);
	}
	
}

//플레이어를 공격
void UCloseAttackEnemyFSM::TickAttack()
{
	//일정 시간이 지나면
	currentTime += GetWorld()->GetDeltaSeconds();

	//일정 시간에 한번씩 공격하기
	if (currentTime >= attackDelayTime)
	{	
		//공격 montage 호출
		me->caEnemyAnim->AnimAttack(TEXT("Attack"));
		
		currentTime = 0;
// 		bAttackPlay = false;
// 		else
// 		{
// 
// 			me->caEnemyAnim->bAttackPlay = true;
// 		}

	}
	//공격범위를 벗어나면 move로 전환
	targetDist = mainTarget->GetDistanceTo(me);

	if (targetDist > attackRange)
	{
		SetState(EEnemyState::MOVE);
	}

}

//로봇을 공격
void UCloseAttackEnemyFSM::TickAttackRobot()
{
	//일정 시간이 지나면
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime >= attackDelayTime) 
	{
		//공격 애니메이션
		me->caEnemyAnim->AnimAttack(TEXT("Attack"));
		currentTime = 0;

	}	
	//로봇과 나의 거리
	razerTargetDist = razerTarget->GetDistanceTo(me);
	//공격 범위를 벗어나면 move로 전환
	if (razerTargetDist > attackRange)
	{
		SetState(EEnemyState::MOVE);
	}
		
}

void UCloseAttackEnemyFSM::TickDamage()
{
	currentTime += GetWorld()->GetDeltaSeconds();

	//죽음 애니메이션 호출
	me->caEnemyAnim->AnimDamage(TEXT("Damage"));

	if (currentTime > 1)
	{
		SetState(EEnemyState::MOVE);
		currentTime = 0;
	}	
	
}

void UCloseAttackEnemyFSM::TickDie()
{
	//bug - delete
	me->caEnemyAnim->bcaEnemyDieEnd = true;

	if (false == me->caEnemyAnim->bcaEnemyDieEnd)
	{
		return;
	}
	//죽음 애니메이션 호출
	me->caEnemyAnim->AnimDie(TEXT("Die"));
	//소멸
	me->Destroy();

}

void UCloseAttackEnemyFSM::OnTakeDamage(float damage)
{	
	maxHP -= damage;
	SetState(EEnemyState::DAMAGE);

	if (maxHP <= 0)
	{
		SetState(EEnemyState::DIE);
		UE_LOG(LogTemp, Warning, TEXT("UCloseAttackEnemyFSM::OnTakeDamage________________________"))
		//me->caEnemyAnim->bcaEnemyDieEnd = false;
	}
	else
	{
		SetState(EEnemyState::MOVE);
	}
}

void UCloseAttackEnemyFSM::OnDamageProcess(int damageValue)
{
	me->PlayAnimMontage(me->caEnemyAnim->enemyMontageFactory, 1, FName("Damage"));
	/*
	UE_LOG(LogTemp, Error, TEXT("Damaged"));
	if (ai)
	{
		ai->StopMovement();
	}
	maxHP -= damageValue;
	if (maxHP <= 0)
	{
		maxHP = 0;
		SetState(EEnemyState::DIE);
		me->caEnemyAnim->AnimDie(TEXT("Die"));
	}
	else
	{
		SetState(EEnemyState::DAMAGE);
		me->caEnemyAnim->AnimDie(TEXT("Damage"));
	}
	*/
}

//플레이어의 체력을 깎을 것이다
void UCloseAttackEnemyFSM::OnHitEvent()
{
	me->caEnemyFSM->bAttackPlay = false;

}

bool UCloseAttackEnemyFSM::UpdateRandomLocation(float radius, FVector& outLocation)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation navLoc;

	bool result = ns->GetRandomPointInNavigableRadius(me->GetActorLocation(), radius, navLoc);

	if (result)
	{
		outLocation = navLoc.Location;
	}

	return result;
}


