// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourseSpawner.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "ResourseSpawnerWithAutofarming.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API AResourseSpawnerWithAutofarming : public AResourseSpawner, public IInteractInterface
{
	GENERATED_BODY()
	
public:

	/** From interface */
	void Interact(AActor* Interactor, bool bShort) override;
	void InteractEnd(AActor* Interactor) override;

	/** Blueprint native events for interaction */
	UFUNCTION(BlueprintNativeEvent)
	void UpgradeSpawnerStart();
	void UpgradeSpawnerStart_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void UpgradeSpawnerEnd();
	void UpgradeSpawnerEnd_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void CollectedResourses();
	void CollectedResourses_Implementation();

	UPROPERTY(BlueprintReadWrite)
	float AutoGathering;



};
