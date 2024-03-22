// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScienceQuestExamineOverlap.generated.h"

UCLASS()
class SUBMARINE_API AScienceQuestExamineOverlap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScienceQuestExamineOverlap();

	// Sphere for tracking examiming time. Player overlaps - time starts to go down
	class USphereComponent* OverlapSphere;

	// Object to examine destroyed bind func
	UFUNCTION()
	void DestroySphere(AActor* DestroyedActor);

};
