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
		//state ����
		bossAnim->animState = state;
	}
	
}

void UBossFSM::MoveState()
{	
	mainTargetDist = me->GetDistanceTo(mainTarget);	
	//�÷��̾ ���� �����δ�
	if (mainTarget)
	{
		FVector mainTargetLoc = mainTarget->GetActorLocation();
		//�÷��̾��� ��ġ�� ã�´�, �̵��Ѵ�
		ai->MoveToLocation(mainTargetLoc);
		
	}	
	//�÷��̾���� �Ÿ��� ���� ���� �����̸� ���� �ִϸ��̼��� �����ϴ� �Լ��� ȣ���Ѵ�
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
	//�÷��̾ �����ϴ� �ִϸ��̼�	
	me->GetCharacterMovement()->MaxWalkSpeed = 0;

	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. ���ð��� �������ϱ�
	if (currentTime > damageDelayTime)
	{		
		// 3. Idle �� ���� ��ȯ
		currentTime = 0;
		state = EBossState::IDLE;
		bossAnim->animState = state;
		//ù ���ݿ��� �� �ִϸ��̼��� ����ϵ���
		bossAnim->bAttackPlay = false;
	}	
}

//ù ������ �ϸ� �ι�° ������ �̰ͺ���
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

void UBossFSM::DamageState() //������ ����ִ� ����
{
	
	// �����ð� ��ٷȴٰ� ���¸� Idle �� ��ȯ�ϰ� �ʹ�.
	
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. ���ð��� �������ϱ�
	if (currentTime > damageDelayTime)
	{		
		// 3. Idle �� ���� ��ȯ
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
	//�÷��̾�� ���ݴ��ϸ� hp�� ���δ�
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
		// �ǰ�
		state = EBossState::DAMAGE;
		bossAnim->animState = state;
		me->PlayAnimMontage(bossAnim->bossAnimFactory, 1, FName("Damage"));
		
		//�ڷ� �и��� P = P0 + vt
		FVector p0 = me->GetActorLocation();
		float easing = 1 - pow(2, -8 * GetWorld()->GetDeltaSeconds());
		FVector vect = me->GetActorForwardVector() * -1 * GetWorld()->GetDeltaSeconds() * 130000 * easing;		
		me->SetActorLocation(p0 + vect);
	}
}

