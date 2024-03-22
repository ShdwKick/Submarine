// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Submarine/GameLogic/Structs.h"
#include "SubmarineGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API USubmarineGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* InfluenceDataTable = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* LivingCreatureTable = nullptr;

	bool GetInfluenceInfoByClass(TSubclassOf<AActor> Actor, FInfluenceInfo& OutInfo);
	bool GetLivingCreatureInfoByClass(TSubclassOf<AActor> Actor, FLivingCreatureInfo& OutInfo);
};
