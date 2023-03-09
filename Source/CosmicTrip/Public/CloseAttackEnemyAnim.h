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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyFSM")
	EEnemyState caEnemyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACloseAttackEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bcaEnemyAttackEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bcaEnemyDieEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay;

	UFUNCTION()
	void AnimNotify_Idle();

	UFUNCTION()
	void AnimNotify_Attack(FName sectionName);

	UFUNCTION()
	void AnimNotify_Die(FName sectionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAnimMontage* enemyMontageFactory;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	EEnemyState state;

	
};
