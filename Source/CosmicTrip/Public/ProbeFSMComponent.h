// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProbeFSMComponent.generated.h"

UENUM(BlueprintType)
enum class EProbeState : uint8
{
	IDLE,
	MOVEMONEY,
	COLLECT,
	MOVEHOME,
	DIE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COSMICTRIP_API UProbeFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProbeFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EProbeState probeState;

	UPROPERTY()
	class AProbeRobot* me;

	UPROPERTY()
	class AMoney* target;

	UPROPERTY()
	class ARefinery* home;

	UPROPERTY(EditAnywhere, Category = "setting")
	float moveSpeed = 100.0f;

private:
	void TickIdle();

	void TickMoveMoney();

	void TickCollect();

	void TickMoveHome();

	void TickDie();

public:
	void SetProbeState(EProbeState next);
};
