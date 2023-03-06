// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "CosmicPlayer.h"
#include "CloseAttackEnemy.h"
#include <NavigationSystem.h>
#include "AIController.h"
#include "CloseAttackEnemyAnim.h"

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

	//내가 갈 수 있는 길 위에 공격 대상이 있는가
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query; //길 찾을 수 있는지 여부
	FAIMoveRequest request;
	request.SetAcceptanceRadius(acceptanceRadius);
	request.SetGoalLocation(mainTarget->GetActorLocation());

	ai->BuildPathfindingQuery(request, query);
	ns->FindPathSync(query);
	FPathFindingResult result = ns->FindPathSync(query);

	if (result.Result == ENavigationQueryResult::Success)
	{
		//me->AddMovementInput(targetDir.GetSafeNormal());
		ai->MoveToLocation(mainTarget->GetActorLocation());
	}
	else
	{
		//타겟이 네비게이션 위에 없다면 랜덤한 위치로 돌아다니도록
		//랜덤한 위치
		auto randLoc = ai->MoveToLocation(randomLocation);
		if (randLoc == EPathFollowingRequestResult::Failed || EPathFollowingRequestResult::AlreadyAtGoal)
		{
			UpdateRandomLocation(randLocationRadius, randomLocation);
		}
	}

	targetDist = mainTarget->GetDistanceTo(me);

	//공격 범위에 들어오면 공격 상태로 전환
	if (targetDist <= attackRange)
	{
		state = EEnemyState::ATTACK;
	}
	else if (targetDist > trackingRange)
	{
		state = EEnemyState::MOVE;
	}
}

void UCloseAttackEnemyFSM::TickAttack()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime >= attackDelayTime)
	{
		float dist = mainTarget->GetDistanceTo(me);

		if (dist > attackRange)
		{
			state = EEnemyState::MOVE;
		}
		else
		{
			currentTime = 0;
			bAttackPlay = false;

			me->caEnemyAnim->bAttackPlay = true;
		}
	}

}

void UCloseAttackEnemyFSM::TickDamage()
{
	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime > 1)
	{
		state = EEnemyState::MOVE;
		currentTime = 0;
	}

	//플레이어의 공격을 받았다면 공격당한 애니메이션 재생
	
}

void UCloseAttackEnemyFSM::TickDie()
{
	//죽는 애니메이션을 재생시킨다
	me->Destroy();

}

void UCloseAttackEnemyFSM::OnTakeDamage(float damage)
{
	maxHP -= damage;

	if (maxHP <= 0)
	{
		state = EEnemyState::DIE;
	}
}

void UCloseAttackEnemyFSM::OnHitEvent()
{
	me->caEnemyFSM->bAttackPlay = false;

	float dist = mainTarget->GetDistanceTo(me);
	if (dist <= attackRange)
	{
		//mainTarge->
		UE_LOG(LogTemp, Warning, TEXT("OnHitEvent() Attack"))
	}
}

bool UCloseAttackEnemyFSM::UpdateRandomLocation(float radius, FVector& outLocation)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation navLoc;

	bool result = ns->GetRandomReachablePointInRadius(me->GetActorLocation(), radius, navLoc);

	if (result)
	{
		outLocation = navLoc.Location;
	}

	return result;
}

