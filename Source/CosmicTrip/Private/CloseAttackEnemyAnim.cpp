// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyAnim.h"
#include "CloseAttackEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCloseAttackEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<ACloseAttackEnemy>(TryGetPawnOwner());
}

void UCloseAttackEnemyAnim::AnimNotify_Idle()
{
}

void UCloseAttackEnemyAnim::AnimNotify_Attack(FName sectionName)
{
	if (!this) return;

	me->caEnemyFSM->OnHitEvent();
		
	me->PlayAnimMontage(enemyMontageFactory, 1, TEXT("Attack"));
}
