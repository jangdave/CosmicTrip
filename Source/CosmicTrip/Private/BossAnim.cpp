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

void UBossAnim::EndAttack()
{
	//me->bossFSM->bAttackPlay = false;
	me->GetCharacterMovement()->MaxWalkSpeed = me->bossFSM->walkSpeed;

	animState = EBossState::IDLE;
	animState = me->bossFSM->state;

}

void UBossAnim::EndDie()
{		
	me->Destroy();
	
}

void UBossAnim::EndNextAttack()
{
	//두번째 공격부터는 이 애니메이션을 재생하도록
	me->GetCharacterMovement()->MaxWalkSpeed = me->bossFSM->walkSpeed;

	animState = EBossState::IDLE;
	animState = me->bossFSM->state;
}

