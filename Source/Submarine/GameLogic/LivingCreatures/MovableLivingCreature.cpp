// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableLivingCreature.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Submarine/GameLogic/LivingCreatures/GameActors/FeedingObject.h"
#include "Components/SphereComponent.h"
#include "Submarine/GameLogic/LivingCreatures/GameActors/MigrationPoint.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NavigationPath.h"


void AMovableLivingCreature::BeginPlay()
{
	Super::BeginPlay();

	InitialPlace = GetActorLocation();
	if (CreatureInfo.MigrationRate > 0.f)
		GetWorldTimerManager().SetTimer(TimerHandle_Migration, this, &AMovableLivingCreature::Migrate, CreatureInfo.MigrationRate, true);

	//AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (IsOverlappingActor(Player))
	//{
	//	EventPlayerDetected(Player);
	//	//PlayerDetected = true;
	//}
}

void AMovableLivingCreature::CheckForFood()
{
	TArray<AActor*> OverlappingActors;
	SphereCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* elem : OverlappingActors)
	{
		AFeedingObject* food = Cast<AFeedingObject>(elem);
		if (food)
		{
			FoodDetected(food);
		}
	}
}

void AMovableLivingCreature::EventPlayerDetected_Implementation(AActor* Player)
{
}

void AMovableLivingCreature::PlayerLost_Implementation()
{
}

void AMovableLivingCreature::FoodDetected_Implementation(AFeedingObject* FoodActor)
{
}

void AMovableLivingCreature::EventTamed_Implementation()
{
}

void AMovableLivingCreature::FoodConsumed()
{
	if (Health < 1)
	{
		if (Health + 1 > 1)
			Health = 1;
		else
			Health += 1;

		UpdateHealthState();
	}

	if (!bIsTamed)
	{
		FeedingCount += 1;
		if (FeedingCount >= CreatureInfo.FeedCountToTame)
		{
			bIsTamed = true;
			EventTamed();
		}
	}

	CheckForFood();

}

void AMovableLivingCreature::Migrate()
{
	if (!bFoodDetected)
	{
		float MigrateChance = 0;
		switch (CurrentState)
		{
		case 2:
			MigrateChance = 0.05;
			break;
		case 1:
			MigrateChance = 0.10;
			break;
		case 0:
			MigrateChance = 0.40;
		default:
			break;
		}

		UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
		bool Migrate = UKismetMathLibrary::RandomBoolWithWeight(MigrateChance);
		if (Migrate)
		{
			// for SphereOverlapActors
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible));
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);

			// for random choosing
			TArray<AMigrationPoint*> MigrationPointsArray;

			TArray<AActor*> FindedMigrationPoints;
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), CreatureInfo.MigratePointsDetectRadius,
				ObjectTypes, AMigrationPoint::StaticClass(), ActorsToIgnore, FindedMigrationPoints);

			int8 i = 0;
			bool bIsFound = false;
			while (i < FindedMigrationPoints.Num() && !bIsFound)
			{
				AMigrationPoint* MigPoint = Cast<AMigrationPoint>(FindedMigrationPoints[i]);
				FPathFindingQuery PathQuery;
				PathQuery.EndLocation = MigPoint->GetActorLocation();
				PathQuery.StartLocation = GetActorLocation();
				PathQuery.SetRequireNavigableEndLocation(true);
				//PathQuery.Owner = this;
				if (MigPoint && navSys->TestPathSync(PathQuery))
				{
					if (MigPoint != CurrentMigrationPoint)
					{

						// Remember point in case we need to choose random point
						// (if we dont find point with better healthInfluence coef)
						MigrationPointsArray.Add(MigPoint);

						// Define fields
						int32 PointHealthInfluence = 0;
						TArray<AActor*> MigrationOverlapActors;

						// Get all potensial influence actors 
						// with the same radius as this creature's sphere collision
						UKismetSystemLibrary::SphereOverlapActors(GetWorld(), MigPoint->GetActorLocation(), SphereCollision->GetScaledSphereRadius(),
							ObjectTypes, AActor::StaticClass(), ActorsToIgnore, MigrationOverlapActors);

						// Get MigratePoint's HealthInfluence coef
						for (AActor* elem : MigrationOverlapActors)
						{
							UInfluenceComponent* myInfluenceComponent = Cast<UInfluenceComponent>(elem->GetComponentByClass(UInfluenceComponent::StaticClass()));
							if (myInfluenceComponent)
							{
								int32 myInfluenceValue = myInfluenceComponent->GetMyInfluenceValue(this);
								PointHealthInfluence += myInfluenceValue;
							}
						}


						// Check if this point have better HealthInfluence coef
						if (PointHealthInfluence > CurrentHealthInfluence)
						{
							//UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), MigPoint->GetActorLocation());
							CurrentMigrationPoint = MigPoint;
							bIsFound = true;
						}
					}

				}
				i++;
			}
			if (!MigrationPointsArray.IsEmpty())
			{
				// We didn't find better point, so we choose random
				if (!bIsFound)
				{
					for (int j = 0; j < MigrationPointsArray.Num(); j++)
					{
						int32 rand = FMath::RandHelper(MigrationPointsArray.Num() - 1);
						AMigrationPoint* MigPoint = MigrationPointsArray[rand];
						FPathFindingQuery PathQuery;
						PathQuery.EndLocation = MigPoint->GetActorLocation();
						PathQuery.StartLocation = GetActorLocation();
						PathQuery.SetRequireNavigableEndLocation(true);

						if (navSys->TestPathSync(PathQuery))
						{
							CurrentMigrationPoint = MigPoint;
							bIsFound = true;
							break;
						}
						else
						{
							MigrationPointsArray.Remove(MigrationPointsArray[rand]);
						}
					}
				}
				if (!bIsFound)
				{
					if (CurrentMigrationPoint)
						MigrationInitiatedBP(CurrentMigrationPoint);
				}
				else
				{
					FVector Loc = GetActorLocation();
					UE_LOG(LogTemp, Error, TEXT("AMovableLivingCreature::Migrate - %s didnt find any migration point, CurrLocation %d %d %d"), *GetActorNameOrLabel(), Loc.X, Loc.Y, Loc.Z);
				}
			}
		}
	}
}

void AMovableLivingCreature::MigrationInitiatedBP_Implementation(AActor* MigrationPoint)
{
}

void AMovableLivingCreature::SphereCollisionOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereCollisionOnBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (bIsAlive)
	{
		AFeedingObject* food = Cast<AFeedingObject>(OtherActor);
		if (food)
		{
			FoodDetected(food);
		}
		else
		{
			AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (OtherActor == Player)
			{
				EventPlayerDetected(Player);
				//PlayerDetected = true;
			}
		}
	}
}

void AMovableLivingCreature::SphereCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereCollisionOnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		PlayerLost();

}
