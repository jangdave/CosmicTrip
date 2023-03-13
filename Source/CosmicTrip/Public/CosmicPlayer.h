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
		
	// 총쏘기
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_Fire;

	//인풋 던짐총
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ThrowGun;

	//인풋 기본총
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grenade;

	//던짐총선택하기
	//void 리턴값이 없는 함수만들때 자료형임
	//void ThrowGun();

	//어떤총을 선택할지에 대한 함수
	void ChooseGun(bool bGrenade);

	//어떤총을 선택했는지 저장하는 변수
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool BeChooseGrenade;

	//기본총을 선택할것이다
	void OnActionGrenade();

	//던짐총을 선택할것이다
	void OnActionThrowGun();


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

	//던짐총 스태틱메시 추가
	UPROPERTY(VisibleAnywhere, Category = ThrowGunMesh)
	class UStaticMeshComponent* ThrowGuncomp;

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

	//폭팔공장
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* bulletImpactFactory;


	//잡기버튼을 누르면 물체를 잡고 싶다.
	//필요속성: 입력액션
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grab;
 
	//필요속성: 잡을범위
 	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float GrabRange =100;

	//잡은물체를 기억하자
	//메모리관리를 위해서 UPROPERTY라는 스마트 포인터를 
	//사용해주는게 좋다. 
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	//잡은녀석이 있는지 여부 기억할변수
	bool IsGrabbed = false;

	//던지면 원하는 방향으로 날아가도록 하고싶다.
	//던질 방향
	FVector ThrowDirection;
	//던질 힘
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPower = 1000;

	// 직전 위치
	FVector PrevPos;

	//잡기시도 기능
	void TryGrab();
	//놓기
	void UnTryGrab();
	//잡고있는중
	void Grabbing();

};
