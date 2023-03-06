// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RazerFSMComponent.generated.h"

UENUM(BlueprintType)
enum class ERazerState : uint8
{
	IDLE,
	MOVE,
	PATROL,
	ATTACK,
	DAMAGE,
	DIE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRIP_API URazerFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URazerFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ERazerState razerState;

	UPROPERTY()
	class ACloseAttackEnemy* enemy;

	UPROPERTY()
	class ARazerRobot* me;

	UPROPERTY()
	class ACosmicPlayer* player;

	UPROPERTY()
	class AAIController* aiRazer;

	void TickIdle();

	void TickMove();

	void TickPatrol();

	void TickAttack();

	void TickDamage();

	void TickDie();

	void OnDamage(int32 damage);

	void SetRazerState(ERazerState next);

	bool bCheckFirst = true;

	bool bCheckEmitter = false;

	FVector dimensions = FVector(300, 0, 0);

	FVector axisVector = FVector(0, 0, 1);

	float angleAxis = 0;

	float multiplier = 1.0f;
};
