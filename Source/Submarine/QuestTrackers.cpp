// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrackers.h"
#include "Submarine/Player/SubmarinePlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"

// Science quests
#include "ScienceQuestExamineOverlap.h"
#include "Components/SphereComponent.h"

// Ecology quests
#include "Submarine/GameLogic/LivingCreatures/LivingCreature.h"

// Buisness quests
#include "Submarine/Player/PlayerInterface.h"

////////////////////////////////////////GARBAGE//////////////////////////////////////////////

void UQuestTracker_Garbage::InitObject(int32 inCountTargetGarbage, class ASubmarinePlayerPawnBase* Player)
{
	CurrentGarbageCount = 0;
	GarbageCountTarget = inCountTargetGarbage;
}

void UQuestTracker_Garbage::GarbagePassed(int32 GarbagePassed)
{
	CurrentGarbageCount += GarbagePassed;
	OnGarbageQuestUpdated.Broadcast(CurrentGarbageCount);

	if (CurrentGarbageCount >= GarbageCountTarget)
		OnQuestCompleted.Broadcast(EAgent::GarbageAgent);
}

void UQuestTracker_Garbage::SetGarbageCount(int32 NewGarbageCount)
{
	CurrentGarbageCount = NewGarbageCount;
}

////////////////////////////////////////SCIENCE//////////////////////////////////////////////

void UQuestTracker_Science::InitObject(TArray<AActor*> inObjectsToInvestigate, float TimeToExamine, float ExplorationRadius)
{
	ExamineTimeTarget = TimeToExamine;

	if (!inObjectsToInvestigate.IsEmpty())
	{
		for (auto elem : inObjectsToInvestigate)
		{
			FVector SpawnLocation = elem->GetActorLocation();
			FRotator SpawnRotation = FRotator(0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AScienceQuestExamineOverlap* Sphere = GetWorld()->SpawnActor<AScienceQuestExamineOverlap>(SpawnLocation, SpawnRotation, SpawnParams);
			if (Sphere && Sphere->OverlapSphere)
			{
				// Set sphere overlap radius
				Sphere->OverlapSphere->SetSphereRadius(ExplorationRadius);
				Sphere->OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &UQuestTracker_Science::CollisionSphereBeginOverlap);
				Sphere->OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &UQuestTracker_Science::CollisionSphereEndOverlap);
				ExamineSpheresOvelap.Add(Sphere);

				// Destroy sphere, if object is destroyed
				elem->OnDestroyed.AddDynamic(Sphere, &AScienceQuestExamineOverlap::DestroySphere);
			}

		}
	}
}

void UQuestTracker_Science::CollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Examining, this, &UQuestTracker_Science::ExamineTimerEvent, 0.1f, true);
}

void UQuestTracker_Science::CollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Examining);
}

void UQuestTracker_Science::ExamineTimerEvent()
{
	ExamineTime += 0.1f;
	if (ExamineTime >= ExamineTimeTarget)
	{
		for (auto elem : ExamineSpheresOvelap)
		{
			elem->Destroy();
		}
		ExamineSpheresOvelap.Empty();
		OnQuestCompleted.Broadcast(EAgent::ScienceAgent);
	}
	else
		OnScienceQuestUpdated.Broadcast(ExamineTime);
}

////////////////////////////////////////ECOLOGY//////////////////////////////////////////////

void UQuestTracker_Ecology::InitObject(TArray<AActor*> CreaturesToHeal)
{
	for (auto elem : CreaturesToHeal)
	{
		ALivingCreature* Creature = Cast<ALivingCreature>(elem);
		if (Creature)
		{
			Creature->OnHealthUpdate.AddDynamic(this, &UQuestTracker_Ecology::CreatureHealthUpdate);
		}
	}
}

void UQuestTracker_Ecology::CreatureHealthUpdate(int32 OldState, int32 NewState)
{
	if (NewState == 2)
		OnQuestCompleted.Broadcast(EAgent::EcologyAgent);
}

////////////////////////////////////////BUISNESS//////////////////////////////////////////////

void UQuestTracker_Buisness::InitObject(TMap<EItem, int32> ResoursesToCollect, class ASubmarinePlayerPawnBase* Player)
{
	TargetResoursesToCollect = ResoursesToCollect;
	PlayerRef = Player;
}

void UQuestTracker_Buisness::PlayerInventoryUpdate(EItem ItemType, int32 NewItemCount, int32 DifferenceValue)
{
	if (TargetResoursesToCollect.Contains(ItemType))
	{
		// Update quest widget
		OnBuisnessQuestUpdated.Broadcast(ItemType, NewItemCount, DifferenceValue);

		// If this resourse is full collected, check for quest completed
		if (NewItemCount >= *TargetResoursesToCollect.Find(ItemType))
		{
			if (PlayerRef->CheckInventoryHaveResourses(TargetResoursesToCollect))
			{
				//TArray<EItem> ResoursesToTake;
				//TargetResoursesToCollect.GetKeys(ResoursesToTake);
				//for (auto elem : ResoursesToTake)
				//{
				//	int32 ResourseCount = ResoursesToTake.Find(elem);
				//	PlayerRef->TakeAwayResourse(elem, ResourseCount);
				//}

				OnQuestCompleted.Broadcast(EAgent::BuisnessAgent);
			}
		}
	}
}
