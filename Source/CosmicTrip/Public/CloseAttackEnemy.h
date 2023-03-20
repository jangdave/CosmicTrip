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

	//�÷��̾��� ü���� ��´�
	UFUNCTION()
	void OnOverlapEnemyEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
