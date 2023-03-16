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
		//state ����
		bossAnim->animState = state;
	}
	//MoveState();

}

void UBossFSM::MoveState()
{
	//�÷��̾ ���� �����δ�
	if (mainTarget)
	{
		FVector mainTargetLoc = mainTarget->GetActorLocation();
		//�÷��̾��� ��ġ�� ã�´�, �̵��Ѵ�
		ai->MoveToLocation(mainTargetLoc);
		
	}

	// �����ð��� �Ǹ� �����ϰ� �ʹ�.
	//TryJump();

	mainTargetDist = me->GetDistanceTo(mainTarget);	
	//�÷��̾���� �Ÿ��� ���� ���� �����̸� ���� �ִϸ��̼��� �����ϴ� �Լ��� ȣ���Ѵ�
	if (attackRange >= mainTargetDist)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 0;
		
		state = EBossState::ATTACK;
		bossAnim->animState = state;
	}
}

//������ �ð��� ���� �ִϸ��̼��� �Ѵ� (�ִϸ��̼� �� �ϸ� �����Ұ�, state�� �ƴ� �׳� �Լ���)
void UBossFSM::TryJump()
{
	//�ȱ� �ӵ��� 0���� ����� ���ڸ����� �����ϵ���
	//me->GetCharacterMovement()->MaxWalkSpeed = 0;
	//������ �ð�
	FTimerHandle jumpTimerHandle;
	float minTime = 4;
	float maxTime = 8;
	float randJumpTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(jumpTimerHandle, this, &UBossFSM::TryJump, randJumpTime);
	me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Jump"));
}

void UBossFSM::AttackState()
{
	//�÷��̾ �����ϴ� �ִϸ��̼�	
	bAttackPlay = true;
	//UE_LOG(LogTemp, Warning, TEXT("UBossFSM:: UBossFSM::AttackState()"))

	//Ÿ�ٰ��� �Ÿ��� ���ݹ����� �����
	if (attackRange < mainTargetDist)
	{
		//�ٽ� �÷��̾ ã����
		state = EBossState::IDLE;
		bossAnim->animState = state;
	}
}

void UBossFSM::DamageState() //������ ����ִ� ����
{
	
	// �����ð� ��ٷȴٰ� ���¸� Idle �� ��ȯ�ϰ� �ʹ�.
	UE_LOG(LogTemp, Warning, TEXT("UBossFSM::DamageState() Change into IDLE"))
	
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. ���ð��� �������ϱ�
	if (currentTime > damageDelayTime)
	{
		// 3. Idle �� ���� ��ȯ
		state = EBossState::IDLE;
		bossAnim->animState = state;
		currentTime = 0;
	}
}

void UBossFSM::DieState()
{
	//hp�� 0 ���ϰ� �Ǹ� �״� �ִϸ��̼�
	me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Die"));	
	
}

void UBossFSM::OnDamageProcess(float attack)
{
	//�÷��̾�� ���ݴ��ϸ� hp�� ���δ�
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
		// �ǰ�
		state = EBossState::DAMAGE;
		bossAnim->animState = state;
		me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Damage"));
	}
}

