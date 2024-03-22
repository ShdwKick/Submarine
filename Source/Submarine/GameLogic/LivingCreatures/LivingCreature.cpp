// Fill out your copyright notice in the Description page of Project Settings.


#include "LivingCreature.h"
#include "Components/SphereComponent.h"
#include "Submarine/GameplayBase/SubmarineGameInstance.h"

// Sets default values
ALivingCreature::ALivingCreature()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating root component...
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Creating sphere collision...
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ALivingCreature::SphereCollisionOnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ALivingCreature::SphereCollisionOnEndOverlap);

	// Creating InfluenceComponent...
	InfluenceComponent = CreateDefaultSubobject<UInfluenceComponent>(TEXT("InfluenceComponent"));
}

// Called when the game starts or when spawned
void ALivingCreature::BeginPlay()
{
	Super::BeginPlay();

	CreatureInit();

	TArray<AActor*> OverlappingActors;
	SphereCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* elem : OverlappingActors)
	{
		UInfluenceComponent* myInfluenceComponent = Cast<UInfluenceComponent>(elem->GetComponentByClass(UInfluenceComponent::StaticClass()));
		if (myInfluenceComponent)
		{
			int32 myInfluenceValue = myInfluenceComponent->GetMyInfluenceValue(this);
			CurrentHealthInfluence += myInfluenceValue;
			//UE_LOG(LogTemp, Warning, TEXT("ALivingCreature::BeginPlay %s"), *FString::FromInt(myInfluenceValue));
		}
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_HealthInfluence, this, &ALivingCreature::HealthInfluenceEvent, CreatureInfo.HealthInfluenceRate, true);
}

// Called every frame
void ALivingCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALivingCreature::CreatureInit()
{
	USubmarineGameInstance* myGI = Cast<USubmarineGameInstance>(GetGameInstance());
	if (myGI)
	{
		FLivingCreatureInfo myInfo;
		if (myGI->GetLivingCreatureInfoByClass(this->GetClass(), myInfo))
		{
			CreatureInfo = myInfo;
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("ALivingCreature::CreatureInit - GameInstance class is not correct"));

	CurrentState = FMath::RoundFromZero(Health);
}

void ALivingCreature::HealthInfluenceEvent()
{
	Health += HealthInfluenceTickValue * CurrentHealthInfluence;
	if (Health < 0)
		Health = 0;
	else if (Health > 2)
		Health = 2;

	UpdateHealthState();
}

void ALivingCreature::UpdateHealthState()
{
	int32 newState = FMath::RoundFromZero(Health);
	if (CurrentState != newState)
	{
		HealthBigChange(CurrentState, newState);
		CurrentState = newState;
	}
}

void ALivingCreature::HealthBigChange_Implementation(int32 OldState, int32 NewState)
{
	if (bIsAlive && NewState == 0)
	{
		bIsAlive = false;
	}
	else if (!bIsAlive && NewState > 0)
	{
		bIsAlive = true;
	}
	OnHealthUpdate.Broadcast(OldState, NewState);
}

void ALivingCreature::SphereCollisionOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UInfluenceComponent* myInfluenceComponent = Cast<UInfluenceComponent>(OtherActor->GetComponentByClass(UInfluenceComponent::StaticClass()));
	if (myInfluenceComponent)
	{
		int32 myInfluenceValue = myInfluenceComponent->GetMyInfluenceValue(this);
		CurrentHealthInfluence += myInfluenceValue;
	}
}

void ALivingCreature::SphereCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UInfluenceComponent* myInfluenceComponent = Cast<UInfluenceComponent>(OtherActor->GetComponentByClass(UInfluenceComponent::StaticClass()));
	if (myInfluenceComponent)
	{
		int32 myInfluenceValue = myInfluenceComponent->GetMyInfluenceValue(this);
		CurrentHealthInfluence -= myInfluenceValue;
	}

}

