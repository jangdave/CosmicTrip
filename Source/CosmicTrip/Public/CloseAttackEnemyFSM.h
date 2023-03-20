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
	
	//멈춰서 플레이어 공격할 범위
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float attackRange = 500;

	//플레이어 추적 거리
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float trackingRange = 4000;	
	
	//로봇 추적 거리
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float trackingRobotRange = 2000;

	//플레이어와 나의 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range")
	float targetDist;
	
	//로봇과 나의 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range")
	float razerTargetDist;
	
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float acceptanceRadius = 5;

	//시간체크
	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDelayTime = 1.5f;
	
	//hp계산할것
	void OnTakeDamage(float damage);

	void OnHitEvent();

	bool bAttackAnimPlay;

	bool bDeathAnimPlay;

	//enemy가 찾아갈 목적지
	FVector wantedLocation; 
	

	//랜덤한 위치
	FVector randomLocation;
	bool UpdateRandomLocation(float radius, FVector& outLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float randLocationRadius = 400;

	//체력
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
