// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmicPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include "Kismet/GameplayStatics.h"
#include "BulletActor.h"

#define PRINTToScreen(msg) UE_LOG(LogTemp, Warning, TEXT("%s"), *msg)

// Sets default values
ACosmicPlayer::ACosmicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;

	//???߰?
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));	

	//?޼? ???̷?Ż?޽???????Ʈ ??????
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));

	//?޼? ???̷?Ż?޽? ?ε??ؼ? ?Ҵ?
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9f, -3.5f, 4.5f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
		LeftHandMesh->SetupAttachment(LeftHand);
	}

	//?????? ???̷?Ż?޽???????Ʈ ??????
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));

	//?????? ???̷?Ż?޽? ?ε??ؼ? ?Ҵ?
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh2.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f, 3.5f, 4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
		RightHandMesh->SetupAttachment(RightHand);
	}

	//???? ??????
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gunMeshComp->SetupAttachment(RightHandMesh);

	//?Ѹ޽???????Ʈ ??????
	ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (GunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(GunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector(3.8f, 5.0f, -10.6f), FRotator(-5, -90, 55));
		gunMeshComp->SetRelativeScale3D(FVector(0.5f));

	}

	//???????? ?????ϰ??ʹ?
	ThrowGuncomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowGuncomp"));
	//???????? ?ֻ??? ??Ʈ??????Ʈ?? ?????ϰ??ʹ?
	ThrowGuncomp->SetupAttachment(RightHandMesh);
	//?????Ѹ޽????θ? ?ҷ??????ʹ?.
	ConstructorHelpers::FObjectFinder<UStaticMesh> ThrowGunMeshComp(TEXT("/Script/Engine.StaticMesh'/Game/GunMesh/free_sci-fi_gun/Cube_059_Cube_056_BASE_0.Cube_059_Cube_056_BASE_0'"));
	//???????? ?????????? ?ҷ??Դٸ?
	if (ThrowGunMeshComp.Succeeded())
	{
		//??????Ʈ?? ?????? ?Ѵ?.
		ThrowGuncomp->SetStaticMesh(ThrowGunMeshComp.Object);
		ThrowGuncomp->SetRelativeLocationAndRotation(FVector(2, 8, -6), FRotator(-40, 180, 14));
		ThrowGuncomp->SetRelativeScale3D(FVector(1.3f));

	}

}

// Called when the game starts or when spawned
void ACosmicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Enhanced Input ????ó??
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
	//?? ???? ???????? ?? ???(?޽?)?? ?????ٰ??̴?
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
	//???????? ?Է¿? ???? ?յ??¿? ?̵??ϰ? ?ʹ?.
	//1.???????? ?Է¿? ????
	FVector2D Axis =Values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);

	//2.?յ? ?¿????? ?????? ?ʿ?
	//FVector Dir(Axis.X, Axis.Y, 0);
	//3. ?̵??ϰ??ʹ?.
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

	//Ÿ?̸Ӹ? ?̿??ؼ? ?ѹ?Ŭ?????Ŀ? ?ڵ????? ?????Ͽ? ???????ϰ??ʹ?.
	//GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &ACosmicPlayer::DoFire, fireInterval, true);


	//???? ?⺻??(?׷????̵???)?̶???
	// gunMeshComp ?? ?ִٸ?
	if(BeChooseGrenade)
	{
		DoFire();
	}
	//?׷????ʴٸ?(??????)
	else
	{

		//?????? ?????ʹ?.
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
	//UGamePlayStatics , UKismetMathLibrary, GetWorld, ???۵?? ã?ƺ???
	//?÷??̾? 1M??
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
	
	/*//?߽???
	FVector Center = RightHand->GetComponentLocation();
	//?浹üũ?ؾ???
	//?浹?? ??ü?? ?????? ?迭
	//?浹 ???? ?ۼ?
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightHand);
	TArray<FOverlapResult> HitObjs;
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), Param);

	//?浹???? ?ʾҴٸ?
	if (bHit == false)
	{
		return;
	}
	//-> ???? ?????? ??ü ?⵵?? ????(????????)
	//???? ?????? ??ü ?ε???
	int Closest = 0;
	for (int i = 0; i < HitObjs.Num(); i++)
	{
		//1.???? ?????? Ȱ??ȭ ?Ǿ? ?ִ? ?༮?? ?Ǵ?
		//->???? ?ε??? ??????Ʈ?? ?????????? ??Ȱ??ȭ ?Ǿ? ?ִٸ?
		if (HitObjs[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			//?????ϰ? ???? ?ʴ?.
			continue;
		}
		//???Ҵ?!
		IsGrabbed = true;

		//2. ???? ?հ? ???? ?????? ?༮?? ?̹??? ?????? ?༮?? ?? ?????? ?༮?? ?ִٸ?
		//->?ʿ??Ӽ? : ???? ???? ?????? ?༮?? ?հ??? ?Ÿ?
		float ClosestDist = FVector::Dist(HitObjs[Closest].GetActor()->GetActorLocation(), Center);
		//->?ʿ??Ӽ? : ?̹??? ?????? ?༮?? ?հ??? ?Ÿ?
		float NextDist = FVector::Dist(HitObjs[i].GetActor()->GetActorLocation(), Center);
		//3.???? ?̹????? ???粨 ???? ?? ?????ٸ?
		if (NextDist < ClosestDist)
		{
			// -> ???? ???????༮???? ?????ϱ?
			Closest = i;
		}
	}

//???? ???Ҵٸ?
	if (IsGrabbed)
	{
	GrabbedObject = HitObjs[Closest].GetComponent();
	//->??ü ???????? ??Ȱ??ȭ
	GrabbedObject->SetSimulatePhysics(false);
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 
	//-> ?տ? ?ٿ?????
	GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);

	//???????? ?ʱⰪ
	PrevPos = RightHand->GetComponentLocation();
	}*/

}

//?????༮?? ?????? ?????ʹ?.
void ACosmicPlayer::UnTryGrab()
{
	if(IsGrabbed == false)
	{
		return;
	}
	//1.???????? ???·? ??ȯ
	IsGrabbed = false;
	//2.?տ??? ?????
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//3.???????? Ȱ??ȭ
	GrabbedObject->SetSimulatePhysics(true);
	//4.?浹???? Ȱ??ȭ
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//??????
	GrabbedObject->AddForce(ThrowDirection * ThrowPower * GrabbedObject->GetMass());
}

void ACosmicPlayer::Grabbing()
{
	if (IsGrabbed == false)
	{
		return;
	}
	//???????? ??????Ʈ
	//Ÿ??(???糪?Ǽ???ġ) - me(?????? ???? ?? ??ġ) 
	ThrowDirection = RightHand->GetComponentLocation() - PrevPos;

	//??????ġ ??????Ʈ
	PrevPos = RightHand->GetComponentLocation();
}
