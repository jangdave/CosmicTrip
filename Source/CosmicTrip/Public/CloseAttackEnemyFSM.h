// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloseAttackEnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{	
	IDLE,
	MOVE,
	MOVETOROBOT,
	ATTACK,
	ATTACKROBOT,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRIP_API UCloseAttackEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCloseAttackEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state;
		
	class ACloseAttackEnemy* me;
	class UCloseAttackEnemyFSM* caEnemyAnim;
	class ACosmicPlayer* mainTarget;
	class ARazerRobot* razerTarget;
	class AAIController* ai;
	
	//���缭 �÷��̾� ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float attackRange = 500;

	//�÷��̾� ���� �Ÿ�
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float trackingRange = 4000;	
	
	//�κ� ���� �Ÿ�
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float trackingRobotRange = 2000;

	//�÷��̾�� ���� �Ÿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range")
	float targetDist;
	
	//�κ��� ���� �Ÿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range")
	float razerTargetDist;
	
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float acceptanceRadius = 5;

	//�ð�üũ
	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDelayTime = 1.5f;
	
	//hp����Ұ�
	void OnTakeDamage(float damage);

	void OnHitEvent();

	bool bAttackAnimPlay;

	bool bDeathAnimPlay;

	//enemy�� ã�ư� ������
	FVector wantedLocation; 
	

	//������ ��ġ
	FVector randomLocation;
	bool UpdateRandomLocation(float radius, FVector& outLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float randLocationRadius = 400;

	//ü��
	float hp;
	float maxHP = 30;		
	
private:
	
	void SetState(EEnemyState next);
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickAttackRobot();
//	void TickDamage();
//	void TickDie();	
	void TickMoveToRobot();
};
