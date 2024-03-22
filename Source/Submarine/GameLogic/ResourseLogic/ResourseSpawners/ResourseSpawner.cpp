// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourseSpawner.h"
#include "Components/BoxComponent.h"
#include "Submarine/GameLogic/ResourseLogic/CollectableResourse.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpawnerWidget.h"

// Sets default values
AResourseSpawner::AResourseSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Creating a spawn zone...
	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
	SpawnZone->SetBoxExtent(FVector(300, 300, 50));
	SpawnZone->SetLineThickness(5);
	SpawnZone->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Creating widget...
	SpawnerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	SpawnerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	SpawnerWidget->AttachToComponent(SpawnZone, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnerWidget->SetRelativeLocation(FVector(0, 0, 250.f));
}

// Called when the game starts or when spawned
void AResourseSpawner::BeginPlay()
{
	Super::BeginPlay();

		// Set stats from data table
		ResourseStats = ResourseType.DataTable->FindRow<FResourseData>(ResourseType.RowName, FString(""));
		if (ResourseStats)
		{
			SpawnSlots = ResourseStats->MaxProduction;
			SpawnDelay = ResourseStats->SpawnTime;
			ResourseClass = ResourseStats->Class;
			if (SpawnSlots > 0)
				GetWorldTimerManager().SetTimer(SpawnTimer, this, &AResourseSpawner::SpawnResourse, SpawnDelay, true);

			// Create widget
			WidgetInstance = CreateWidget<USpawnerWidget>(GetWorld());
			SpawnerWidget->SetWidget(WidgetInstance);
			WidgetInstance->SetIcon(ResourseStats->Icon);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("%s has no ResourseType data."), *GetName());
	}

// Called every frame
void AResourseSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResourseSpawner::SpawnResourse()
{
		FVector BoundsOrigin;
		FVector BoundsExtend;
		float SphereRadius;
		UKismetSystemLibrary::GetComponentBounds(SpawnZone, BoundsOrigin, BoundsExtend, SphereRadius);

		FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(BoundsOrigin, FVector(BoundsExtend.X, BoundsExtend.Y, 0));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;

		FRotator SpawnRotation = FRotator(0.f, UKismetMathLibrary::RandomFloatInRange(-360, 360), 0.f);
		ACollectableResourse* NewResourse = GetWorld()->SpawnActor<ACollectableResourse>(ResourseClass, SpawnPoint, SpawnRotation, SpawnParams);
		if (NewResourse)
		{
			SpawnSlots--;
			NewResourse->OnDestroyed.AddDynamic(this, &AResourseSpawner::PlayerCollectedResourse);

			if (SpawnSlots == 0)
				GetWorldTimerManager().ClearTimer(SpawnTimer);
		}

}

void AResourseSpawner::PlayerCollectedResourse(AActor* CollectedResourse)
{
	if (SpawnSlots == 0)
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AResourseSpawner::SpawnResourse, SpawnDelay, true);
	SpawnSlots++;
}

