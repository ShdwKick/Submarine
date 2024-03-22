// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SubmarineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API ASubmarineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	class UDataTable* SienceQuests = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	class UDataTable* EcologyQuests = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	class UDataTable* BuisnessQuests = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	class UDataTable* GarbageQuests = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation")
	class UDataTable* GMRepGoodsTable = nullptr;
};
