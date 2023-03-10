// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

UCLASS()
class COSMICTRIP_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* gunMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossSettings")
	float walkSpeed = 300.f;

	class UCloseAttackEnemyFSM* bossFSM;

	UPROPERTY(EditDefaultsOnly)
	class UCloseAttackEnemyAnim* bossAnim;

};
