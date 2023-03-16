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
	animState = EBossState::IDLE;
	me->bossFSM->bAttackPlay = false;
}

void UBossAnim::EndDie()
{
	
	me->Destroy();
}

void UBossAnim::DieDestroy()
{

}

