// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CosmicPlayer.generated.h"


UCLASS()
class COSMICTRIP_API ACosmicPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACosmicPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//필요속성: 이동속도, 입력액션, 입력매핑 컨텍스트
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_VRInput;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	//사용할 이동처리함수
	void Move(const FInputActionValue& Values);

	//마우스 입력처리

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;

	//회전처리 함수
	void Turn(const FInputActionValue& Values);

	//총쏘기 기능
	void OnActionFirePressed();
	void OnActionFireReleased();

	void DoFire();

	//마우스 왼쪽버튼을 누르면 총알공장에서 총알을 만들어서 총구위치에 배치하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	class ABulletActor* bullet;

protected:
	
	//카메라
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* VRCamera;

	//컨트롤러 
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* RightHand;

	//사용할 손 모델
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* RightHandMesh;

	//총을 생성한다.
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere)
	float fireInterval = 0.5f;
	FTimerHandle fireTimerHandle;
	

};
