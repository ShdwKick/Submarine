// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LivingCreature.h"
#include "MovableLivingCreature.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API AMovableLivingCreature : public ALivingCreature
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	//
	//* TAMING LOGIC *//
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsTamed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFoodDetected = false;

	UFUNCTION(BlueprintCallable)
	void CheckForFood();

	// How many times player feeded this group
	int32 FeedingCount = 0;

	UFUNCTION(BlueprintNativeEvent)
	void EventTamed();

	UFUNCTION(BlueprintNativeEvent)
	void EventPlayerDetected(AActor* Player);

	UFUNCTION(BlueprintNativeEvent)
	void PlayerLost();
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//bool PlayerDetected = false;

	UFUNCTION(BlueprintNativeEvent)
	void FoodDetected(class AFeedingObject* FoodActor);

	UFUNCTION(BlueprintCallable)
	void FoodConsumed();

	//
	//* MIGRATION LOGIC *//
	//
	FTimerHandle TimerHandle_Migration;

	FVector InitialPlace;

	void Migrate();

	UFUNCTION(BlueprintNativeEvent)
	void MigrationInitiatedBP(AActor* MigrationPoint);

	class AMigrationPoint* CurrentMigrationPoint = nullptr;

	void SphereCollisionOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	void SphereCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
