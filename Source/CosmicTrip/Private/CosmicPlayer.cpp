// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmicPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include "Kismet/GameplayStatics.h"
#include "BulletActor.h"
#include <Sound/SoundBase.h>

#define PRINTToScreen(msg) UE_LOG(LogTemp, Warning, TEXT("%s"), *msg)

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
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
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

	//던짐총을 생성하고싶다
	ThrowGuncomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowGuncomp"));
	//던짐총을 최상단 루트컴포넌트에 부착하고싶다
	ThrowGuncomp->SetupAttachment(RightHandMesh);
	//던짐총메쉬경로를 불러오고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> ThrowGunMeshComp(TEXT("/Script/Engine.StaticMesh'/Game/GunMesh/free_sci-fi_gun/Cube_059_Cube_056_BASE_0.Cube_059_Cube_056_BASE_0'"));
	//던짐총을 성공적으로 불러왔다면
	if (ThrowGunMeshComp.Succeeded())
	{
		//오브젝트로 등록을 한다.
		ThrowGuncomp->SetStaticMesh(ThrowGunMeshComp.Object);
		ThrowGuncomp->SetRelativeLocationAndRotation(FVector(2, 8, -6), FRotator(-40, 180, 14));
		ThrowGuncomp->SetRelativeScale3D(FVector(1.3f));

	}

	//사운드를 만들고싶다.
	GunFireSound = CreateDefaultSubobject<USoundBase>(TEXT("GunFireSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> fireSound(TEXT("/Script/Engine.SoundWave'/Game/CosmicVR/Sound/Sound_Ddock_cutegun_.Sound_Ddock_cutegun_'"));
	if (fireSound.Succeeded())
	{
		GunFireSound = (fireSound.Object);
	


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

	ChooseGun(true);
}

// Called every frame
void ACosmicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Grabbing();
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
		InputSystem->BindAction(IA_ThrowGun, ETriggerEvent::Started, this, &ACosmicPlayer::OnActionThrowGun);
		InputSystem->BindAction(IA_Grenade, ETriggerEvent::Started, this, &ACosmicPlayer::OnActionGrenade);
		InputSystem->BindAction(IA_Fire, ETriggerEvent::Started, this, &ACosmicPlayer::OnActionFirePressed);
	}
}


void ACosmicPlayer::ChooseGun(bool bGrenade)
{
	//if(BeChooseGrenade == false && bGrenade == true)
	BeChooseGrenade = bGrenade;
	//이 총을 선택했을 때 어떤총(메쉬)를 보여줄것이다
	gunMeshComp->SetVisibility(bGrenade);
	ThrowGuncomp->SetVisibility(!bGrenade);

}

void ACosmicPlayer::OnActionGrenade()
{
	ChooseGun(true);
}

void ACosmicPlayer::OnActionThrowGun()
{
	ChooseGun(false);
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


	//만약 기본총(그랜에이드건)이라면
	// gunMeshComp 가 있다면
	if(BeChooseGrenade)
	{
		DoFire();
		UGameplayStatics::PlaySoundAtLocation(this, GunFireSound, GetActorLocation(),GetActorRotation());
	}
	//그렇지않다면(던짐총)
	else
	{

		//물건을 잡고싶다.
		TryGrab();

	}
}

void ACosmicPlayer::OnActionFireReleased()
{
	//UnTryGrab();
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

void ACosmicPlayer::TryGrab()
{
	FHitResult hitResult;
	FVector start;
	FVector end;
	//GetWorld()->LineTraceSingleByChannel(hitResult,start)
	
	/*//중심점
	FVector Center = RightHand->GetComponentLocation();
	//충돌체크해야함
	//충돌한 물체들 기록할 배열
	//충돌 질의 작성
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightHand);
	TArray<FOverlapResult> HitObjs;
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), Param);

	//충돌하지 않았다면
	if (bHit == false)
	{
		return;
	}
	//-> 가장 가까운 물체 잡도록 하자(검출과정)
	//가장 가까운 물체 인덱스
	int Closest = 0;
	for (int i = 0; i < HitObjs.Num(); i++)
	{
		//1.물리 기능이 활성화 되어 있는 녀석만 판단
		//->만약 부딪힌 컴포넌트가 물리기능이 비활성화 되어 있다면
		if (HitObjs[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			//검출하고 싶지 않다.
			continue;
		}
		//잡았다!
		IsGrabbed = true;

		//2. 현재 손과 가장 가까운 녀석과 이번에 검출할 녀석과 더 가까운 녀석이 있다면
		//->필요속성 : 현재 가장 가까운 녀석과 손과의 거리
		float ClosestDist = FVector::Dist(HitObjs[Closest].GetActor()->GetActorLocation(), Center);
		//->필요속성 : 이번에 검출할 녀석과 손과의 거리
		float NextDist = FVector::Dist(HitObjs[i].GetActor()->GetActorLocation(), Center);
		//3.만약 이번에가 현재꺼 보다 더 가깝다면
		if (NextDist < ClosestDist)
		{
			// -> 가장 가까운녀석으로 변경하기
			Closest = i;
		}
	}

//만약 잡았다면
	if (IsGrabbed)
	{
	GrabbedObject = HitObjs[Closest].GetComponent();
	//->물체 물리기능 비활성화
	GrabbedObject->SetSimulatePhysics(false);
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 
	//-> 손에 붙여주자
	GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);

	//프리포즈 초기값
	PrevPos = RightHand->GetComponentLocation();
	}*/

}

//잡은녀석이 있으면 놓고싶다.
void ACosmicPlayer::UnTryGrab()
{
	if(IsGrabbed == false)
	{
		return;
	}
	//1.잡지않은 상태로 전환
	IsGrabbed = false;
	//2.손에서 떼어내기
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//3.물리기능 활성화
	GrabbedObject->SetSimulatePhysics(true);
	//4.충돌기능 활성화
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//던지기
	GrabbedObject->AddForce(ThrowDirection * ThrowPower * GrabbedObject->GetMass());
}

void ACosmicPlayer::Grabbing()
{
	if (IsGrabbed == false)
	{
		return;
	}
	//던질방향 업데이트
	//타겟(현재나의손위치) - me(이전에 나의 손 위치) 
	ThrowDirection = RightHand->GetComponentLocation() - PrevPos;

	//이전위치 업데이트
	PrevPos = RightHand->GetComponentLocation();
}
