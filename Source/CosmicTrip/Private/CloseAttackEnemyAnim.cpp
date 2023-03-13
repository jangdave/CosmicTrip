// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyAnim.h"
#include "CloseAttackEnemy.h"
#include "CloseAttackEnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCloseAttackEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<ACloseAttackEnemy>(TryGetPawnOwner());

	//meFSM = Cast<ACloseAttackEnemyFSM>()
}

// void UCloseAttackEnemyAnim::AnimAttack(FName sectionName)
// {
// 	//if (!this) return;
// 
// 	me->caEnemyFSM->OnHitEvent();
// 		
// 	me->PlayAnimMontage(enemyMontageFactory, 1, TEXT("Attack"));
// }
// 
// void UCloseAttackEnemyAnim::AnimDamage(FName sectionName)
// {
// 	//if (!this) return;
// 	me->PlayAnimMontage(enemyMontageFactory, 1, TEXT("Damage"));
// }
// 
// void UCloseAttackEnemyAnim::AnimDie(FName sectionName)
// {
// 	//if (!this) return;
// 	me->PlayAnimMontage(enemyMontageFactory, 1, TEXT("Die"));
// }

void UCloseAttackEnemyAnim::EndAttack()
{
	me->GetCharacterMovement()->MaxWalkSpeed = me->walkSpeed;
	me->caEnemyFSM->state = EEnemyState::IDLE;
	me->caEnemyFSM->bAttackAnimPlay = false;
}

void UCloseAttackEnemyAnim::EndDamage()
{
	me->GetCharacterMovement()->MaxWalkSpeed = me->walkSpeed;
}

void UCloseAttackEnemyAnim::EndDie()
{
	me->Destroy();
}


