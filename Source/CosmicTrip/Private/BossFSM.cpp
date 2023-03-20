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
	case EBossState::NEXTATTACK:
		NextAttackState();
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

	me->GetCharacterMovement()->MaxWalkSpeed = 0;

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > 0.5f)
	{
		currentTime = 0;
		me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

		state = EBossState::MOVE;
		//state 갱신
		bossAnim->animState = state;
	}
	
}

void UBossFSM::MoveState()
{	
	mainTargetDist = me->GetDistanceTo(mainTarget);	
	//플레이어를 향해 움직인다
	if (mainTarget)
	{
		FVector mainTargetLoc = mainTarget->GetActorLocation();
		//플레이어의 위치를 찾는다, 이동한다
		ai->MoveToLocation(mainTargetLoc);
		
	}	
	//플레이어와의 거리가 공격 범위 이하이면 공격 애니메이션을 실행하는 함수를 호출한다
	if (attackRange >= mainTargetDist && bossAnim->bAttackPlay == true)
	{				
		state = EBossState::ATTACK;
		bossAnim->animState = state;

		UE_LOG(LogTemp, Warning, TEXT("UBossFSM::MoveState() >> Attack State"))
	}
	else if (attackRange >= mainTargetDist && bossAnim->bAttackPlay == false)
	{
		state = EBossState::NEXTATTACK;
		bossAnim->animState = state;
		
	}

}

void UBossFSM::AttackState()
{	
	//플레이어를 공격하는 애니메이션	
	me->GetCharacterMovement()->MaxWalkSpeed = 0;

	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. 대기시간이 끝났으니까
	if (currentTime > damageDelayTime)
	{		
		// 3. Idle 로 상태 전환
		currentTime = 0;
		state = EBossState::IDLE;
		bossAnim->animState = state;
		//첫 공격에만 이 애니메이션을 재생하도록
		bossAnim->bAttackPlay = false;
	}	
}

//첫 공격을 하면 두번째 공격은 이것부터
void UBossFSM::NextAttackState()
{
	//me->GetCharacterMovement()->MaxWalkSpeed = 0;
	
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > damageDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBossFSM::MoveState() >>>>>>>> NEXT ATTACK State"))
		currentTime = 0;	
		state = EBossState::IDLE;
		bossAnim->animState = state;
	}
}

void UBossFSM::DamageState() //시점을 잡아주는 역할
{
	
	// 일정시간 기다렸다가 상태를 Idle 로 전환하고 싶다.
	
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. 대기시간이 끝났으니까
	if (currentTime > damageDelayTime)
	{		
		// 3. Idle 로 상태 전환
		currentTime = 0;
		state = EBossState::MOVE;
		bossAnim->animState = state;
	}
	
	bossAnim->bBossEndDie = false;
}

void UBossFSM::DieState()
{	
	me->GetCharacterMovement()->MaxWalkSpeed = 0;
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
		me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Die"));
	
		bossAnim->bBossEndDie = false;
	}
	else
	{
		// 피격
		state = EBossState::DAMAGE;
		bossAnim->animState = state;
		me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Damage"));
		
		//뒤로 밀리기 P = P0 + vt
		FVector p0 = me->GetActorLocation();
		float easing = 1 - pow(2, -8 * GetWorld()->GetDeltaSeconds());
		FVector vect = me->GetActorForwardVector() * -1 * GetWorld()->GetDeltaSeconds() * 130000 * easing;		
		me->SetActorLocation(p0 + vect);
	}
}

