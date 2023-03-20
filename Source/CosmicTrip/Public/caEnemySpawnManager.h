// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "caEnemySpawnManager.generated.h"

UCLASS()
class COSMICTRIP_API AcaEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcaEnemySpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//랜덤한 시간동안
	UPROPERTY(EditDefaultsOnly)
	float minTime = 1;

	UPROPERTY(EditDefaultsOnly)
	float maxTime = 7;

	//랜덤한 장소에서
	UPROPERTY(EditDefaultsOnly)
	TArray<class AActor*> spawnList;

	int prevRandIndex = -1;

	//Enemy 생성할 시간
	float spawningTime;
	FTimerHandle timerHandlespawncaEnemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ACloseAttackEnemy> caEnemyFactory;	
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ABoss> bossFactory;

	void SpawnEnemy();

	//스폰할 적 갯수 제한
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int limitedEnemyCount = 2;

	//함수 실행될 때마다 스폰되는 갯수 저장할 변수
	int EnemyCount = 0;
public:

	//Enemy가 다 스폰 되면 boss가 등장하도록
	void SpawnBoss();
	//Boss 스폰할 시간
	float bossSpawnTime;
	FTimerHandle timerHandleSpawnBoss;
	 


};
