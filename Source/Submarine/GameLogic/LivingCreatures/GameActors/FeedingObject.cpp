// Fill out your copyright notice in the Description page of Project Settings.


#include "FeedingObject.h"
#include "Submarine/Player/PlayerInterface.h"

// Sets default values
AFeedingObject::AFeedingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFeedingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFeedingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFeedingObject::Interact(AActor* Interactor, bool bShort)
{
	//UGameplayStatics::PlaySound2D(GetWorld(), CleanUpSound);
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	if (Player)
	{
		Player->CollectedResourse(EItem::Food, 1, 0);
		Destroy();
	}
}

