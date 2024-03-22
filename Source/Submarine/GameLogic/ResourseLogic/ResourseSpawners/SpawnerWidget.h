// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "SpawnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API USpawnerWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

public:


	UPROPERTY(BlueprintReadWrite)
	UTexture2D* MyIcon;

	UPROPERTY()
	UImage* ResourseIcon;
		
	void SetIcon(UTexture2D* NewIcon);
};
