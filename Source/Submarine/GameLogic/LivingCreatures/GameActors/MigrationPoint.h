// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MigrationPoint.generated.h"

UCLASS()
class SUBMARINE_API AMigrationPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMigrationPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<TSubclassOf<class AMovableLivingCreature>> PossibleCreatures;

	// Returns level, in bounds of wich this point works
	int32 GetMyLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 myLevel = 1;

};
