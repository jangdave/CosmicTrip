// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeftPlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API ULeftPlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 hp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 maxhp;
};
