// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Robot.h"
#include "ProbeRobot.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API AProbeRobot : public ARobot
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProbeRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	class UProbeFSMComponent* probeFSM;

};
