// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmicPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include "Kismet/GameplayStatics.h"
#include "BulletActor.h"

// Sets default values
ACosmicPlayer::ACosmicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;

	//손추가
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	//왼손 스켈레탈메시컴포넌트 만들기
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));

	//왼손 스켈레탈메시 로드해서 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9f, -3.5f, 4.5f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
		LeftHandMesh->SetupAttachment(LeftHand);
	}

	//오른손 스켈레탈메시컴포넌트 만들기
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));

	//오른손 스켈레탈메시 로드해서 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh2.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f, 3.5f, 4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
		RightHandMesh->SetupAttachment(RightHand);
	}

	//총을 만든다
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gunMeshComp->SetupAttachment(RightHandMesh);

	//총메시컴포넌트 만들기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (GunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(GunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector(3.8f, 5.0f, -10.6f), FRotator(-5, -90, 55));
		gunMeshComp->SetRelativeScale3D(FVector(0.5f));

	}

}

// Called when the game starts or when spawned
void ACosmicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Enhanced Input 사용처리
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		auto LocalPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_VRInput, 0);
		}

	}

	bullet = Cast<ABulletActor>(UGameplayStatics::GetActorOfClass(GetWorld(), bulletFactory));
}

// Called every frame
void ACosmicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACosmicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		//binding for moving
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACosmicPlayer::Move);
		InputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &ACosmicPlayer::Turn);
	}

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACosmicPlayer::OnActionFirePressed);

	//PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ACosmicPlayer::OnActionFireReleased);
}

void ACosmicPlayer::Move(const FInputActionValue& Values)
{
	//사용자의 입력에 따라 앞뒤좌우 이동하고 싶다.
	//1.사용자의 입력에 따라
	FVector2D Axis =Values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);

	//2.앞뒤 좌우라는 방향이 필요
	//FVector Dir(Axis.X, Axis.Y, 0);
	//3. 이동하고싶다.
// 	//4.P=P0+vt
// 	FVector P0 = GetActorLocation();
// 	FVector vt = Dir * MoveSpeed * GetWorld()->DeltaTimeSeconds;
// 	FVector  P = P0 + vt;
//	SetActorLocation(P);
}

void ACosmicPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}


void ACosmicPlayer::OnActionFirePressed()
{

	//타이머를 이용해서 한번클릭이후에 자동으로 생성하여 나가게하고싶다.
	//GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &ACosmicPlayer::DoFire, fireInterval, true);

	// 더블클릭을하면 총알이 안나가는 방법 해결 
	DoFire();
}

void ACosmicPlayer::OnActionFireReleased()
{
	//GetWorldTimerManager().ClearTimer(fireTimerHandle);
}

void ACosmicPlayer::DoFire()
{
	//SpawnActor
	//UGamePlayStatics , UKismetMathLibrary, GetWorld, 구글등에서 찾아보기
	//플레이어 1M앞
	//FVector loc= GetActorLocation()+GetActorForwardVector()*1; 

	//FTransform t = gunMeshComp->GetSocketTransform(TEXT("Muzzle"));
	FTransform Trans = gunMeshComp->GetSocketTransform(FName(TEXT("FirePosition")));
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, Trans, Params);

}



