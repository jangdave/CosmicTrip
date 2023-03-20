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
	UFUNCTION(BlueprintCallable)
	void EndAttack();
	UFUNCTION(BlueprintCallable)
	void EndDie();
	UFUNCTION(BlueprintCallable)
	void EndNextAttack();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	EBossState animState;

	//EndDie 애니메이션 딜레이 시키기 위해서
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bBossEndDie;

	//첫 Attack Animation 재생하는지 확인하기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay = true;

	//다음 공격 Animation 재생하는지 확인하기
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	bool bNextAttackPlay = true;

};
