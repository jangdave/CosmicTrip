	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CloseAttackEnemyFSM.h"
#include "CloseAttackEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API UCloseAttackEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyAnim")
	EEnemyState caEnemyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACloseAttackEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bcaEnemyAttackEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bcaEnemyDieEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay;

	//로봇 혹은 플레이어를 향해 걸을지 선택할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChooseWalk;

	UFUNCTION()
	void AnimAttack(FName sectionName);

	UFUNCTION()
	void AnimDamage(FName sectionName);

	UFUNCTION()
	void AnimDie(FName sectionName);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAnimMontage* enemyMontageFactory;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	EEnemyState state;

	
};
