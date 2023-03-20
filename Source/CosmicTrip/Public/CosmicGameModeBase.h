// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CosmicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API ACosmicGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACosmicGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
