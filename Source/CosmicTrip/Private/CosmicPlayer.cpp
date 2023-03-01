// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmicPlayer.h"
#include <Camera/CameraComponent.h>
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"

// Sets default values
ACosmicPlayer::ACosmicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void ACosmicPlayer::BeginPlay()
{
	Super::BeginPlay();

// 	//Enhanced Input 사용처리
// 	auto PC = Cast<APlayerController>(GetWorld()->GetFirstLocalPlayerFromController());
// 
// 	if (PC)
// 	{
// 		//Local player
// 		auto localPlayer = PC->GetLocalPlayer();
// 		auto subSystem = ULocalPlayer::GetSubsystem < UEnhancedInputLocalPlayerSubsystem(localPlayer);
// 		if (subSystem)
// 		{
// 			subSystem->AddMappingContext(IMC_VRInput, 0);
// 		}
// 	}
// 	
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
		//Binding
		InputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &ACosmicPlayer::Turn);
	}

}

void ACosmicPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}