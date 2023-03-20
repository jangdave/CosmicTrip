// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	NEXTATTACK,
	DAMAGE,
	DIE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRIP_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//PlayAnimMontage�� �غ���
	UPROPERTY()
	class ABoss* me;
	UPROPERTY()
	class UBossAnim* bossAnim;
	UPROPERTY()
	class ACosmicPlayer* mainTarget;
	UPROPERTY()
	class AAIController* ai;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Boss")
	EBossState state;

	//�ð�üũ
	float currentTime = 0;
	//Damage ����Ǵ� ����
	float damageDelayTime = 2;
	//Die ����Ǵ� ����

	//Boss HP ����ϱ�
	float hp;
	float maxHP = 100;

	//Boss�� Player�� �����ϴ� �Ÿ� ����
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float attackRange = 300;

	//�÷��̾�� Boss ������ �Ÿ�
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float mainTargetDist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossSettings")
	float walkSpeed = 350.f;

	void IdleState();
	void MoveState();
	void AttackState();
	void NextAttackState();
	void DamageState();
	void DieState();

	UFUNCTION()
	void OnDamageProcess(float attack);



};
