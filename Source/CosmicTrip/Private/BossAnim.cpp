// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "Boss.h"
#include "BossFSM.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBossAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<ABoss>(TryGetPawnOwner());
}

void UBossAnim::EndDamage()
{
	me->GetCharacterMovement()->MaxWalkSpeed = me->bossFSM->walkSpeed;
	//me->bossFSM->bDamagePlay = false;
	animState = EBossState::IDLE;
	animState = me->bossFSM->state;
}

// void UBossAnim::EndAttack()
// {
// 	//me->bossFSM->bAttackPlay = false;
// 	me->GetCharacterMovement()->MaxWalkSpeed = me->bossFSM->walkSpeed;
// 	//�����ϰ� ���� ���� ��ġ : EndAttack anim notify�� ȣ��� ����
// 	animState = EBossState::IDLE;
// 	animState = me->bossFSM->state;
// }

void UBossAnim::EndDie()
{		
	me->Destroy();
	
}

void UBossAnim::EndNextAttack()
{
	//�ι�° ���ݺ��ʹ� �� �ִϸ��̼��� ����ϵ���
	me->GetCharacterMovement()->MaxWalkSpeed = me->bossFSM->walkSpeed;

	animState = EBossState::IDLE;
	animState = me->bossFSM->state;
}

void UBossAnim::EndJumpAttack()
{
	//�����ϰ� �̵��� ��ġ�� ���� �����´�.
	FVector landingLoc = me->GetActorLocation();
	//�� ��ġ�� ���� ���� �� ��ġ�� ������Ʈ�Ѵ�.
	//me->SetActorLocation(landingLoc);	
	
}

