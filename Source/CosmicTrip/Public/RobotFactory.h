// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotFactory.generated.h"

UCLASS()
class COSMICTRIP_API ARobotFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "component")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "component")
	class UStaticMeshComponent* meshComp;

	UPROPERTY()
	class ARazerRobot* razer;

	UPROPERTY(EditDefaultsOnly, Category = "factory")
	TSubclassOf<class ARazerRobot> razerFactory;

	void SpawnRazerRobot();
};
