// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class COSMICTRIP_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//충돌체, 외관, 이동컴포넌트
	UPROPERTY(EditAnywhere)
	class USphereComponent* spherComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* movementComp;

	UFUNCTION() //언리얼에서 인식할수있는 형식
	void OnDie();

	UFUNCTION()
	void BulletPower(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class UNiagaraComponent* exploEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* boomEffect;

	UFUNCTION()
	void StartBoom(FVector loc);
};
