// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CloseAttackEnemy.generated.h"

UCLASS()
class COSMICTRIP_API ACloseAttackEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACloseAttackEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* gunMesh;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* axeBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemySettings")
	float walkSpeed = 250.f;

	class UCloseAttackEnemyFSM* caEnemyFSM;

	UPROPERTY(EditDefaultsOnly)
	class UCloseAttackEnemyAnim* caEnemyAnim;

	UPROPERTY(VisibleAnyWhere, Category = "Sound")
	class USoundBase* axeAttackSound;

	//플레이어의 체력을 깎는다
	UFUNCTION()
	void OnOverlapEnemyEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//적 스폰 niagara
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawn")
	class UNiagaraSystem* spawnEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawn")
	class UNiagaraComponent* spawnComp;

	UFUNCTION()
	void SpawnEffect();
	

};
