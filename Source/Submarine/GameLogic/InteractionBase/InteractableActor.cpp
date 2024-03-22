// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInteractableActor::InteractConditions(AActor* Interactor)
{
	return true;
}

void AInteractableActor::InteractionRefused(AActor* Interactor)
{
}

//When player pawn collides with this actor. Calls blueprintEvent
void AInteractableActor::Interact(AActor* Interactor, bool bShort)
{
		Interaction(bShort);
}

void AInteractableActor::InteractEnd(AActor* Interactor)
{
	InteractionEnd();
}

/* 
* Implementations
*/

void AInteractableActor::Interaction_Implementation(bool bShort)
{
}

void AInteractableActor::InteractionEnd_Implementation()
{
}
