// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Robot.h"
#include "NiagaraComponent.h"
#include "RazerRobot.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API ARazerRobot : public ARobot
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARazerRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	class URazerFSMComponent* razerFSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* fireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraComponent* niagaraFireComp;

	UFUNCTION()
	void StartFire();

	UPROPERTY()
	class ABoss* boss;

	UPROPERTY()
	class ACosmicPlayer* player;

	UPROPERTY()
	FVector spawnLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* impactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraComponent* impactComp;
};