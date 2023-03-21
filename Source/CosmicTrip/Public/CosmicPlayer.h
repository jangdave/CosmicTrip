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

	//�ʿ�Ӽ�: �̵��ӵ�, �Է¾׼�, �Է¸��� ���ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_VRInput;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Hand;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
		
	// �ѽ��
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_Fire;

	//��ǲ ������
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ThrowGun;

	//��ǲ �⺻��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grenade;
	
	//����� ���������� ���� �Լ�
	void ChooseGun(bool bGrenade);

	//����� �����ߴ��� �����ϴ� ����
	bool BeChooseGrenade;

	//�⺻���� �����Ұ��̴�
	void OnActionGrenade();

	//�������� �����Ұ��̴�
	void OnActionThrowGun();

	//����� �̵�ó���Լ�
	void Move(const FInputActionValue& Values);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;

	//ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& Values);

	void OnActionFirePressed();

	//�ѽ�� ���
	void DoFire();

	UPROPERTY(VisibleAnywhere, Category = ThrowGunMesh)
	class UStaticMeshComponent* ThrowGuncomp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	class ABulletActor* bullet;                   

	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRCamera")
	class UCameraComponent* VRCamera;
	
	//CameraShake
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> camShakeFactory;

	UPROPERTY(EditAnywhere)
	class UCameraShakeBase* canShakeInstance;

	//��Ʈ�ѷ� 
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* LeftHand;

	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* RightHand;

	//����� �� ��
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MotionController")
	class USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MotionController")
	class USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* RightAim;

	//���� �����Ѵ�.
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere)
	float fireInterval = 0.5f;

	FTimerHandle fireTimerHandle;

	//����
	UPROPERTY(VisibleAnyWhere, Category = "Sound")
	class USoundBase* GunFireSound;

	//����ư�� ������ ��ü�� ��� �ʹ�.
	//�ʿ�Ӽ�: �Է¾׼�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grab;
 
	//�ʿ�Ӽ�: ��������
 	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float GrabRange =100;

	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	//�����༮�� �ִ��� ���� ����Һ���
	bool IsGrabbed = false;

	//������ ���ϴ� �������� ���ư����� �ϰ�ʹ�.
	//���� ����
	FVector ThrowDirection;

	//���� ��
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPower = 1000;

	// ���� ��ġ
	FVector PrevPos;

	//���õ� ���
	void TryGrab();

	//����
	void UnTryGrab();

	//����ִ���
	void Grabbing();

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	class UWidgetInteractionComponent* itemWidget;

	//UI ����
	UFUNCTION()
	void OpenItemWidget();

	UFUNCTION()
	void OnPlayerDamage(int32 damage);

	int32 MaxHP = 50;

	int32 HP;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UWidgetComponent* playWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UWidgetComponent* leftWidget;

	UPROPERTY()
	class UPlayUserWidget* user_UI;

	UPROPERTY()
	class ULeftPlayWidget* leftUser_UI;

	bool bFirst;
};
