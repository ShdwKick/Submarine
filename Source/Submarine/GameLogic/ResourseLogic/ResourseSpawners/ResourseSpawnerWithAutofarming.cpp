// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourseSpawnerWithAutofarming.h"
#include "Submarine/Player/PlayerInterface.h"

void AResourseSpawnerWithAutofarming::Interact(AActor* Interactor, bool bShort)
{
	if (bShort)
	{
		if (AutoGathering > 0)
		{
			IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
			if (Player)
			{
				Player->CollectedResourse(ResourseStats->ItemType, AutoGathering, 0);
			}
			CollectedResourses();
			AutoGathering = 0;
		}
	}
	else
		UpgradeSpawnerStart();
}

void AResourseSpawnerWithAutofarming::InteractEnd(AActor* Interactor)
{
	UpgradeSpawnerEnd();
}


/** Implementations for Blueprint Native Events*/

void AResourseSpawnerWithAutofarming::UpgradeSpawnerStart_Implementation()
{
}

void AResourseSpawnerWithAutofarming::UpgradeSpawnerEnd_Implementation()
{
}

void AResourseSpawnerWithAutofarming::CollectedResourses_Implementation()
{
}
