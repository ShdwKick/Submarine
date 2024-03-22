// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.generated.h"

UENUM(BlueprintType)
enum class EItem : uint8
{
	NONE UMETA(DisplayName = "None"),
	Bronze UMETA(DisplayName = "Metal Bronze"),
	Gold UMETA(DisplayName = "Metal Gold"),
	Platinum UMETA(DisplayName = "Metal Platinum"),
	BlueCrystall UMETA(DisplayName = "Crystall Blue"),
	GreenCrystall UMETA(DisplayName = "Crystall Green"),
	RedCrystall UMETA(DisplayName = "Crystall Red"),
	Chemicals UMETA(DisplayName = "Chemicals"),
	Food UMETA(DisplayName = "Food")
};

UENUM(BlueprintType)
enum class ECurrency : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	Money UMETA(DisplayName = "Money"),
	ScienceRep UMETA(DisplayName = "Science Reputation"),
	EcologyRep UMETA(DisplayName = "Ecology Reputation"),
	BusinessRep UMETA(DisplayName = "Buisness Reputation"),
	GarbageRep UMETA(DisplayName = "Garbage Reputation")
};

USTRUCT(BlueprintType)
struct FResourseData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItem ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Class = nullptr;

	UPROPERTY(EditAnywhere)
	int32 MaxProduction = 0;

	UPROPERTY(EditAnywhere)
	float SpawnTime = 0.f;

	UPROPERTY(EditAnywhere)
	float EnergyConsumption = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuyCost = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellCost = 100;
};

USTRUCT(BlueprintType)
struct FLivingCreatureInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
	TSubclassOf<AActor> Class = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
	int32 myLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Taming")
	int32 FeedCountToTame = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthInfluenceRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	TArray<UTexture*> StateTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
	float MigrationRate = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
	float MigratePointsDetectRadius = 700.0f;
};

USTRUCT(BlueprintType)
struct FInfluenceInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AActor>, int32> Influence;
};

USTRUCT(BlueprintType)
struct FMinibaseBuildingInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EItem, int32> BuildCost;
};

