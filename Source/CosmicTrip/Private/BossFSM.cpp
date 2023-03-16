// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "Boss.h"
#include <Kismet/GameplayStatics.h>
#include "BossAnim.h"
#include "AIController.h"
#include "CosmicPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
// #include "GameFramework/Actor.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABoss>(GetOwner());	
	ai = Cast<AAIController>(me->GetController());
	bossAnim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());
	mainTarget = Cast< ACosmicPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACosmicPlayer::StaticClass()));
	hp = maxHP;


	state = EBossState::IDLE;
	
}

// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EBossState::IDLE:
		IdleState();
		break;
	case EBossState::MOVE:
		MoveState();
		break;
	case EBossState::ATTACK:
		AttackState();
		break;
	case EBossState::DAMAGE:
		DamageState();
		break;
	case EBossState::DIE:
		DieState();
		break;

	}

}

void UBossFSM::IdleState()
{
	//Find Player
	if (!mainTarget)
	{
		return;
	}
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > 0.5f)
	{
		currentTime = 0;
		state = EBossState::MOVE;
		//state 갱신
		bossAnim->animState = state;
	}
	//MoveState();

}

void UBossFSM::MoveState()
{
	//플레이어를 향해 움직인다
	if (mainTarget)
	{
		FVector mainTargetLoc = mainTarget->GetActorLocation();
		//플레이어의 위치를 찾는다, 이동한다
		ai->MoveToLocation(mainTargetLoc);
		
	}

	// 일정시간이 되면 점프하고 싶다.
	//TryJump();

	mainTargetDist = me->GetDistanceTo(mainTarget);	
	//플레이어와의 거리가 공격 범위 이하이면 공격 애니메이션을 실행하는 함수를 호출한다
	if (attackRange >= mainTargetDist)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 0;
		
		state = EBossState::ATTACK;
		bossAnim->animState = state;
	}
}

//랜덤한 시간에 점프 애니메이션을 한다 (애니메이션 다 하면 수정할것, state가 아닌 그냥 함수로)
void UBossFSM::TryJump()
{
	//걷기 속도를 0으로 만들고 제자리에서 점프하도록
	//me->GetCharacterMovement()->MaxWalkSpeed = 0;
	//랜덤한 시간
	FTimerHandle jumpTimerHandle;
	float minTime = 4;
	float maxTime = 8;
	float randJumpTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(jumpTimerHandle, this, &UBossFSM::TryJump, randJumpTime);
	me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Jump"));
}

void UBossFSM::AttackState()
{
	//플레이어를 공격하는 애니메이션	
	bAttackPlay = true;
	//UE_LOG(LogTemp, Warning, TEXT("UBossFSM:: UBossFSM::AttackState()"))

	//타겟과의 거리가 공격범위를 벗어나면
	if (attackRange < mainTargetDist)
	{
		//다시 플레이어를 찾도록
		state = EBossState::IDLE;
		bossAnim->animState = state;
	}
}

void UBossFSM::DamageState() //시점을 잡아주는 역할
{
	
	// 일정시간 기다렸다가 상태를 Idle 로 전환하고 싶다.
	UE_LOG(LogTemp, Warning, TEXT("UBossFSM::DamageState() Change into IDLE"))
	
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. 대기시간이 끝났으니까
	if (currentTime > damageDelayTime)
	{
		// 3. Idle 로 상태 전환
		state = EBossState::IDLE;
		bossAnim->animState = state;
		currentTime = 0;
	}
}

void UBossFSM::DieState()
{
	//hp가 0 이하가 되면 죽는 애니메이션
	me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Die"));	
	
}

void UBossFSM::OnDamageProcess(float attack)
{
	//플레이어에게 공격당하면 hp가 깎인다
	hp -= attack;
	currentTime = 0;
	ai->StopMovement();
	if (hp <= 0)
	{
		state = EBossState::DIE;
		bossAnim->animState = state;
		
	}
	else
	{
		// 피격
		state = EBossState::DAMAGE;
		bossAnim->animState = state;
		me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Damage"));
	}
}

