// Fill out your copyright notice in the Description page of Project Settings.


#include "Garbage.h"
#include "Kismet/GameplayStatics.h"
#include "Submarine/Player/PlayerInterface.h"

// Sets default values
AGarbage::AGarbage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating InfluenceComponent...
	InfluenceComponent = CreateDefaultSubobject<UInfluenceComponent>(TEXT("InfluenceComponent"));

}

// Called when the game starts or when spawned
void AGarbage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGarbage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGarbage::Interact(AActor* Interactor, bool bShort)
{
	UGameplayStatics::PlaySound2D(GetWorld(), CleanUpSound);
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	if (Player)
	{
		Player->CollectGarbage();
		Destroy();
	}
}

bool AGarbage::InteractConditions(AActor* Interactor)
{
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	if (Player)
	{
		if (Player->CanTakeMoreGarbage())
			return true;
		else
		{
			Player->ShowNotification(FText::FromString(TEXT("Нельзя поднять больше, сдадите накопившийся мусор еноту на базе!")));
		}
	}
	return false;
}

