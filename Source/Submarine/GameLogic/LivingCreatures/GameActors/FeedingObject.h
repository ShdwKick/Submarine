// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "FeedingObject.generated.h"

UCLASS()
class SUBMARINE_API AFeedingObject : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFeedingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// From interact interface
// 
// Interact event. Short - press, not Short (long) - hold
	void Interact(AActor* Interactor, bool bShort) override;

};
