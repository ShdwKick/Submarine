// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class SUBMARINE_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//From interface
	virtual void Interact(AActor* Interactor, bool bShort) override;
	virtual void InteractEnd(AActor* Interactor) override;
	virtual bool InteractConditions(AActor* Interactor) override;
	virtual void InteractionRefused(AActor* Interactor) override;

	//Interaction
	UFUNCTION(BlueprintNativeEvent)
	void Interaction(bool Short);
	void Interaction_Implementation(bool Short);

	UFUNCTION(BlueprintNativeEvent)
	void InteractionEnd();
	void InteractionEnd_Implementation();

};
