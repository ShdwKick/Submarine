// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SubmarinePlayerControllerBase.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SUBMARINE_API ASubmarinePlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:

	void BeginPlay() override;


	//Quests
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
	class UReputationAndRepQuestsControl* QuestsControlObject = nullptr;

	//HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InGameWidget;

	//HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* HUD = nullptr;
};
