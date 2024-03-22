// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Submarine/GameLogic/Structs.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "ResourseSpawner.generated.h"

UCLASS()
class SUBMARINE_API AResourseSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourseSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Stats*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle ResourseType;
	FResourseData* ResourseStats;

	/** Spawning */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimerHandle SpawnTimer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnDisabledRange = 100;

	void SpawnResourse();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SpawnSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ResourseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* SpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* SpawnerWidget;
	class USpawnerWidget* WidgetInstance;


	// Binds to spawned resourse's "OnDestroy" event
	UFUNCTION()
	void PlayerCollectedResourse(AActor* CollectedResourse);

};
