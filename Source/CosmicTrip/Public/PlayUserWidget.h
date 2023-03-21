// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COSMICTRIP_API UPlayUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CallBlood();

	UFUNCTION(BlueprintImplementableEvent)
	void CallEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float gameTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayAnim;
};
