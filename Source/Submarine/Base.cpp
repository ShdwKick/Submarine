// Fill out your copyright notice in the Description page of Project Settings.


#include "Base.h"
#include "Components/SphereComponent.h"
#include "Submarine/Player/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));


	// Creating sphere collision...
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABase::CollisionSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABase::CollisionSphereEndOverlap);
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorldTimerManager().SetTimerForNextTick(this, &ABase::CheckIsPlayerOverlapped);

}

void ABase::CheckIsPlayerOverlapped()
{
	if (CheckIsBaseEnabled())
	{
		TArray<AActor*> OverlappingActors;
		SphereCollision->GetOverlappingActors(OverlappingActors);
		for (auto elem : OverlappingActors)
		{
			IPlayerInterface* myInterface = Cast<IPlayerInterface>(elem);
			if (myInterface)
			{
				myInterface->BaseOverlapChanged(true);
			}
		}
	}
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABase::CheckIsBaseEnabled()
{
	return true;
}

void ABase::CollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CheckIsBaseEnabled())
	{
		IPlayerInterface* myInterface = Cast<IPlayerInterface>(OtherActor);
		if (myInterface)
		{
			myInterface->BaseOverlapChanged(true);
		}
	}
}

void ABase::CollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CheckIsBaseEnabled())
	{
		IPlayerInterface* myInterface = Cast<IPlayerInterface>(OtherActor);
		if (myInterface)
		{
			myInterface->BaseOverlapChanged(false);
		}
	}
}

