// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Submarine/GameLogic/LivingCreatures/Influence/InfluenceComponent.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "Garbage.generated.h"

UCLASS()
class SUBMARINE_API AGarbage : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGarbage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Components

	UInfluenceComponent* InfluenceComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* CleanUpSound = nullptr;

	// From interact interface
	// 
	// Interact event. Short - press, not Short (long) - hold
	void Interact(AActor* Interactor, bool bShort) override;
	bool InteractConditions(AActor* Interactor) override;
};
