// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"

UCLASS()
class COSMICTRIP_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoney();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "component")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "component")
	class UStaticMeshComponent* meshComp;
};
