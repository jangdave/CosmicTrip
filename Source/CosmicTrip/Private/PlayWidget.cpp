// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayWidget.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APlayWidget::APlayWidget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("playWidget"));
	playWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayWidget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

