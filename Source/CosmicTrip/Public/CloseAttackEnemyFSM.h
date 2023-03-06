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
	ATTACK,
	DAMAGE,
	DIE,
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

	EEnemyState state;

	class ACloseAttackEnemy* me;
	class ACosmicPlayer* mainTarget;
	//class ARobot* robots;
	class AAIController* ai;


	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float attackRange = 400;
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float trackingRange = 900;

	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float acceptanceRadius = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float targetDist;

	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDelayTime = 2;
 
	
	//hp계산할것
	void OnTakeDamage(float damage);

	void OnHitEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay;

	//랜덤한 위치
	FVector randomLocation;
	bool UpdateRandomLocation(float radius, FVector& outLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float randLocationRadius = 400;

	//체력
	float hp;
	float maxHP = 30;

	
	
private:
	
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();
		
};
