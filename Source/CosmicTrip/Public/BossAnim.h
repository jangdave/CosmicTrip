// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	class ABoss* me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* bossAnimFactory;

	UFUNCTION(BlueprintCallable)
	void EndDamage();
// 	UFUNCTION(BlueprintCallable)
// 	void EndAttack();
	UFUNCTION(BlueprintCallable)
	void EndDie();
	UFUNCTION(BlueprintCallable)
	void EndNextAttack();

	UFUNCTION(BlueprintCallable)
	void EndJumpAttack();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	EBossState animState;

	//EndDie �ִϸ��̼� ������ ��Ű�� ���ؼ�
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bBossEndDie;

	//ù Attack Animation ����ϴ��� Ȯ���ϱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay = true;

	

};
